#include "window.h"
#include <QtGui>
#include <iostream>
#include "addjob.h"
#include "addoverlord.h"
#include "jobtreeitem.h"
#include <fstream>
#include "deadlinedjob.h"
#include "deadlinedjoboverlord.h"
#include <algorithm>
#include <QLocale>
#include "editjob.h"

Window::Window()
{
    QLocale::setDefault(QLocale(QLocale::English));
    maxInt=0;
    setupUi(this);
    treeWidget->headerItem()->setText(3, QApplication::translate("MainWindow", "", 0));
    treeWidget->header()->resizeSection(1,100);
    treeWidget->header()->resizeSection(2,200);
    treeWidget->header()->resizeSection(3,29);
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        treeWidget->header()->setResizeMode(0,QHeaderView::Stretch);
    #else
        treeWidget->header()->setSectionResizeMode(0,QHeaderView::Stretch);
    #endif

    QDir MainFolder(QDir::homePath()+"/.prioriter");
    if (!MainFolder.exists()) {
        setupProgram(MainFolder);
    }
    std::ifstream oldJobs;
    oldJobs.open(QDir::toNativeSeparators(MainFolder.absoluteFilePath("savedJobs/Jobs.txt")).toStdString(),std::ifstream::in);
    //QList<QTreeWidgetItem*> addToTree;
    Priority jobPriorities(UNKNOWN);
    long nearestDeadline(31536000);
    if (oldJobs.is_open()) {
        QDateTime now(QDateTime::currentDateTime());
        std::string jobDescription;
        while (!oldJobs.eof()) {
            std::getline(oldJobs,jobDescription);
            if (jobDescription.size()>2) {
                Job* newJob;
                switch (jobDescription[0]) {
                    case 'd': {
                        newJob = new deadlinedJob(jobDescription.substr(2,jobDescription.size()-2));
                        long deadl(now.secsTo(newJob->due())*newJob->getWeight());
                        if (deadl>0) {
                            nearestDeadline=std::min(nearestDeadline,deadl);
                        }
                        break;
                    }
                    case 'o': {
                        jobOverlord* newJobOverlord  = new jobOverlord(jobDescription.substr(2,jobDescription.size()-2));
                        overlordList.push_back(newJobOverlord);
                        std::vector<jobOverlord*> jobTypes(newJobOverlord->getJobTypes());;
                        for (auto jobSaves: jobTypes) {
                            overlordList.push_back(jobSaves);
                        }
                        newJob = newJobOverlord;
                        break;
                    }
                    case 'p': {
                        deadlinedJobOverlord* newJobOverlord = new deadlinedJobOverlord(jobDescription.substr(2,jobDescription.size()-2));
                        overlordList.push_back(newJobOverlord);
                        std::vector<jobOverlord*> jobTypes(newJobOverlord->getJobTypes());;
                        for (auto jobSaves: jobTypes) {
                            overlordList.push_back(jobSaves);
                        }
                        newJob = newJobOverlord;
                        long deadl(now.secsTo(newJob->due())*newJob->getWeight());
                        if (deadl>0) {
                            nearestDeadline=std::min(nearestDeadline,deadl);
                        }
                        break;
                    }
                    case 'j': {
                        newJob = new Job(jobDescription.substr(2,jobDescription.size()-2));
                        break;
                    }
                default: throw std::invalid_argument("unrecognised job type : ");
                }
                jobs.push_back(new jobTreeItem(this,newJob));
                jobs.back()->update();
                connect(jobs.back(),SIGNAL(deleteSignal(jobTreeItem*)),this,SLOT(removeJob(jobTreeItem*)));
                jobPriorities=std::max(jobPriorities,newJob->getMaxPriority());
            }
        }
        oldJobs.close();
    }
    qSort(jobs.begin(),jobs.end(),jobTreeItem::sortFunc);
    for (auto job : jobs) {
        job->addWidget(treeWidget);
    }
    createActions();

    createTrayIcon(jobPriorities,nearestDeadline);
    signalMapper = new QSignalMapper (this);
    createMenu();
    trayIcon->show();

    setWindowTitle(tr("Prioriter"));
    resize(400, 300);
    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(300000);
}

Window::~Window () {
    for (auto item : jobs) {
        delete item;
    }
}

void Window::setVisible(bool visible)
{
    QMainWindow::setVisible(visible);
}

void Window::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

#if defined(Q_WS_X11)
bool Window::eventFilter(QObject *, QEvent *event)
{
    switch(event->type()) {
    case QEvent::ToolTip:
        /*if (jitToolTipCheckBox->isChecked()) {
            QString timeString = QTime::currentTime().toString();
            trayIcon->setToolTip(tr("Current Time: %1").arg(timeString));
        }*/
        std::cout << "tool tip" << std::endl;
        break;
    case QEvent::Wheel: {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int delta = wheelEvent->delta() > 0 ? 1 : -1;
        /*int index = (iconComboBox->currentIndex() + delta) % iconComboBox->count();
        iconComboBox->setCurrentIndex(index);*/
        std::cout << "wheel : " << delta << std::endl;
        break;
    }
    default:
        break;
    }
    return false;
}
#endif

void Window::createTrayIcon(Priority jobPriorities,long nearestDeadline)
{
    trayIconMenu = new QMenu(this);

    trayIcon = new QSystemTrayIcon(this);
    switch(jobPriorities) {
        case UNKNOWN: trayIcon->setIcon(QIcon(":/NoJobs")); break;
        case LOW: trayIcon->setIcon(QIcon(":/LowJobs")); break;
        case MEDIUM: 
        case HIGH: {
            if (nearestDeadline<7200) {
                trayIcon->setIcon(QIcon(":/RedHighJobs")); break;
            }
            else if (nearestDeadline<172800) {
                trayIcon->setIcon(QIcon(":/HighJobs")); break;
            }
            else {
                trayIcon->setIcon(QIcon(":/MediumJobs")); break;
            }
        }
    }
    QByteArray category = qgetenv("SNI_CATEGORY");
    if (!category.isEmpty()) {
        trayIcon->setProperty("_qt_sni_category", QString::fromLocal8Bit(category));
    }
    trayIcon->setContextMenu(trayIconMenu);

#if defined(Q_WS_X11)
    trayIcon->installEventFilter(this);
#endif
}

void Window::createActions() {
    connect(actionAdd_Job, SIGNAL(triggered()) ,this, SLOT(AddJobPopUp()));
    connect(actionAdd, SIGNAL(triggered()) ,this, SLOT(AddOverlordPopUp()));
    connect(actionQuit, SIGNAL(triggered()) ,qApp, SLOT(quit()));
    connect(addButton, SIGNAL(clicked()) ,this, SLOT(AddJobPopUp()));
    connect(dotButton, SIGNAL(clicked()) ,this, SLOT(AddOverlordPopUp()));
}

void Window::Save() {
    QDir MainFolder(QDir::homePath()+"/.prioriter/savedJobs");

    std::ofstream writeFile;
    writeFile.open(QDir::toNativeSeparators(MainFolder.absoluteFilePath("Jobs.txt")).toStdString(),std::ofstream::out);
    writeFile.close();
    for (int i(0); i<jobs.size(); ++i) {
        jobs[i]->getJob()->writeJob(QDir::toNativeSeparators(MainFolder.absoluteFilePath("Jobs.txt")));
    }
}

void Window::QuitAndSave() {
    Save();
    qApp->quit();
}

void Window::createMenu() {
    trayIconMenu->addAction(actionAdd_Job);
    trayIconMenu->addAction(actionAdd);

    if (jobs.size()>0) {
        trayIconMenu->addSeparator();
        for (auto jobItem: jobs) {
            jobItem->addToMenu(trayIconMenu);
        }
    }

    trayIconMenu->addSeparator();
    showWindow = new QAction(tr("&Edit Jobs"),this);
    connect(showWindow,SIGNAL(triggered()),this,SLOT(openWindow()));
    trayIconMenu->addAction(showWindow);
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(QuitAndSave()));
    trayIconMenu->addAction(quitAction);
    connect(trayIconMenu,SIGNAL(aboutToShow()),this,SLOT(update()));
}

void Window::openWindow() {
    QMainWindow::setVisible(true);
    activateWindow();
}

void Window::rebuildTrayIconMenu() {
    trayIconMenu->clear();
    trayIconMenu->addAction(actionAdd_Job);
    trayIconMenu->addAction(actionAdd);

    Priority jobPriorities(UNKNOWN);
    long nearestDeadline(31536000);
    if (jobs.size()>0) {
        QDateTime now(QDateTime::currentDateTime());
        trayIconMenu->addSeparator();
        for (auto jobItem: jobs) {
            jobItem->addToMenuAgain(trayIconMenu);
            jobPriorities=std::max(jobPriorities,jobItem->getJob()->getPriority());
            if (jobItem->getJob()->hasFixedDeadline()) {
                long deadl(now.secsTo(jobItem->getJob()->due())*jobItem->getJob()->getWeight());
                if (deadl>0) nearestDeadline=std::min(nearestDeadline,deadl);
            }
        }
    }

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(showWindow);
    trayIconMenu->addAction(quitAction);
    switch(jobPriorities) {
        case UNKNOWN: trayIcon->setIcon(QIcon(":/NoJobs")); break;
        case LOW: trayIcon->setIcon(QIcon(":/LowJobs")); break;
        case MEDIUM: 
        case HIGH: {
            if (nearestDeadline<7200) {
                trayIcon->setIcon(QIcon(":/RedHighJobs")); break;
            }
            else if (nearestDeadline<172800) {
                trayIcon->setIcon(QIcon(":/HighJobs")); break;
            }
            else {
                trayIcon->setIcon(QIcon(":/MediumJobs")); break;
            }
        }
    }
}

void Window::rebuildTree() {
    for (auto job: jobs) {
        job->saveExpansionState();
    }
    treeWidget->clear();
    qSort(jobs.begin(),jobs.end(),jobTreeItem::sortFunc);
    for (auto job : jobs) {
        job->addWidget(treeWidget);
    }
}

void Window::removeJob(jobTreeItem* jobPtr) {
    //std::cout << "removing " << jobPtr->getJob()->Name() << std::endl;
    treeWidget->takeTopLevelItem(treeWidget->indexOfTopLevelItem(jobPtr->getWidget()));
    const Job* jobItself(jobPtr->getJob());
    if (jobItself->isOverlord()) {
        const jobOverlord* jobO(dynamic_cast<const jobOverlord*>(jobItself));
        overlordList.erase(std::remove(overlordList.begin(),overlordList.end(),jobO),overlordList.end());
        jobO->complete();
    }
    if (jobs.size()>1) {
        trayIconMenu->removeAction(jobPtr->getMenuItem());
        jobs.removeOne(jobPtr);
        delete jobPtr;
        for (auto jobItem:jobs) {
            jobItem->update();
        }
    }
    else {
        jobs.removeOne(jobPtr);
        delete jobPtr;
    }
    rebuildTree();
    rebuildTrayIconMenu();
    Save();
    timer->start(300000);
}

void Window::AddJobPopUp(jobOverlord* index) {
    AddJob jobPopup(overlordList,index);
    jobPopup.exec();
    if (jobPopup.result()) {
        bool addedToOverlord(jobPopup.addToOverlord(overlordList));

        Job* newJob(jobPopup.getJob());

        jobTreeItem *manager (new jobTreeItem(this,newJob));

        if (!addedToOverlord) {
            manager->addToMenu(trayIconMenu);
            jobs.push_back(manager);
            connect(jobs.back(),SIGNAL(deleteSignal(jobTreeItem*)),this,SLOT(removeJob(jobTreeItem*)));
        }
        else {
            jobTreeItem* parent (overlordList[jobPopup.getPosInOverlord()]->getParent());
            parent->addChild(manager);
        }
        qSort(jobs.begin(),jobs.end(),jobTreeItem::sortFunc);
        rebuildTree();
        rebuildTrayIconMenu();
    }
    Save();
    timer->start(300000);
}

void Window::AddOverlordPopUp(jobOverlord *index) {
    AddOverlord jobPopup(overlordList,index);
    jobPopup.exec();
    if (jobPopup.result()) {
        bool addedToOverlord(jobPopup.addToOverlord(overlordList));
        Job* newJob(jobPopup.getJob());

        jobTreeItem *manager (new jobTreeItem(this,newJob));
        if (!addedToOverlord) {
            manager->addToMenu(trayIconMenu);
            jobs.push_back(manager);
            connect(jobs.back(),SIGNAL(deleteSignal(jobTreeItem*)),this,SLOT(removeJob(jobTreeItem*)));
        }
        else {
            jobTreeItem* parent (overlordList[jobPopup.getPosInOverlord()]->getParent());
            parent->addChild(manager);
        }
        qSort(jobs.begin(),jobs.end(),jobTreeItem::sortFunc);

        rebuildTree();
        rebuildTrayIconMenu();
    }
    Save();
    timer->start(300000);
}

void Window::rebuild() {
    rebuildTree();
    rebuildTrayIconMenu();
}

void Window::update() {
    bool changed(false);
    for (auto jobItem: jobs) {
        changed=changed || jobItem->update();
    }
    qSort(jobs.begin(),jobs.end(),jobTreeItem::sortFunc);
    rebuildTree();
    rebuildTrayIconMenu();
}

void Window::setupProgram(QDir const& MainFolder) {
    MainFolder.mkpath("savedJobs");
#ifdef __linux__
    /*
    // /usr/share/applications
    QDir DesktopFiles(QDir::homePath()+"/.local/share/applications");
    std::ofstream desktopFile(QDir::toNativeSeparators(DesktopFiles.absoluteFilePath("To Do App.desktop")).toStdString(),std::ifstream::out);
    desktopFile << "[Desktop Entry]\nName=To Do App\nComment=System tray based app to keep track of jobs and prioritise them.\nExec=";
    desktopFile << QDir::currentPath().toStdString();
    desktopFile << "\nIcon=";// /usr/share/todo/Sknife.ico
    desktopFile << "\nTerminal=false\nType=Application";
    desktopFile.close();*/
#elif _WIN32
#endif
}

void Window::editJob(jobTreeItem* jobManager) {
    EditJob editor(this,overlordList,jobManager);
    editor.exec();
    if (editor.result()) {
        rebuildTree();
        rebuildTrayIconMenu();
        Save();
    }
}

void Window::addJob(jobTreeItem* manager) {
    jobs.push_back(manager);
    manager->setParent(nullptr);
    connect(jobs.back(),SIGNAL(deleteSignal(jobTreeItem*)),this,SLOT(removeJob(jobTreeItem*)));
    qSort(jobs.begin(),jobs.end(),jobTreeItem::sortFunc);
}

void Window::extractJob(jobTreeItem* manager) {
    jobs.removeOne(manager);
}

void Window::addOverlord(jobOverlord* overlord) {
    overlordList.push_back(overlord);
}

void Window::extractOverlord(jobOverlord* overlord) {
    overlordList.erase(std::remove(overlordList.begin(),overlordList.end(),overlord),overlordList.end());
}
