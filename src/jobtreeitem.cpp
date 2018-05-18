#include "jobtreeitem.h"
#include <QVariant>
#include <exception>
#include <QMessageBox>
#include "joboverlord.h"
#include "deadlinedjob.h"
#include "deadlinedjoboverlord.h"
#include <QApplication>
#include <QToolButton>
#include <QString>
#include "window.h"

jobTreeItem::jobTreeItem(Window* w, Job* j)
    :mainWindow(w), myWidget(new QTreeWidgetItem(2)), parent(nullptr),isExpanded(false) {
    myJob=j;
    myJob->saveParent(this);
    connect(this,SIGNAL(killInWindow(jobTreeItem*)),mainWindow,SLOT(removeJob(jobTreeItem*)));
    connect(this,SIGNAL(editJobPopup(jobTreeItem*)),mainWindow,SLOT(editJob(jobTreeItem*)));
}

jobTreeItem::jobTreeItem(Window* w, Job* j,const QStringList & strings)
    :mainWindow(w), myWidget(new QTreeWidgetItem(strings,2)), parent(nullptr),isExpanded(false) {
    myJob=j;
    myJob->saveParent(this);
    connect(this,SIGNAL(killInWindow(jobTreeItem*)),mainWindow,SLOT(removeJob(jobTreeItem*)));
    connect(this,SIGNAL(editJobPopup(jobTreeItem*)),mainWindow,SLOT(editJob(jobTreeItem*)));
}

jobTreeItem::jobTreeItem(Window* w, Job* j,QTreeWidget * parent)
    :mainWindow(w), myWidget(new QTreeWidgetItem(parent, 2)), parent(nullptr),isExpanded(false) {
    myJob=j;
    myJob->saveParent(this);
    connect(this,SIGNAL(killInWindow(jobTreeItem*)),mainWindow,SLOT(removeJob(jobTreeItem*)));
    connect(this,SIGNAL(editJobPopup(jobTreeItem*)),mainWindow,SLOT(editJob(jobTreeItem*)));
}

jobTreeItem::jobTreeItem(Window* w, Job* j,QTreeWidget * parent, const QStringList & strings)
    :mainWindow(w), myWidget(new QTreeWidgetItem(parent, strings, 2)), parent(nullptr),isExpanded(false) {
    myJob=j;
    myJob->saveParent(this);
    connect(this,SIGNAL(killInWindow(jobTreeItem*)),mainWindow,SLOT(removeJob(jobTreeItem*)));
    connect(this,SIGNAL(editJobPopup(jobTreeItem*)),mainWindow,SLOT(editJob(jobTreeItem*)));
}

jobTreeItem::jobTreeItem(Window* w, Job* j,QTreeWidget * parent, QTreeWidgetItem * preceding)
    :mainWindow(w), myWidget(new QTreeWidgetItem(parent, preceding, 2)), parent(nullptr),isExpanded(false) {
    myJob=j;
    myJob->saveParent(this);
    connect(this,SIGNAL(killInWindow(jobTreeItem*)),mainWindow,SLOT(removeJob(jobTreeItem*)));
    connect(this,SIGNAL(editJobPopup(jobTreeItem*)),mainWindow,SLOT(editJob(jobTreeItem*)));
}

jobTreeItem::jobTreeItem(Window* w, Job* j,QTreeWidgetItem * parent)
    :mainWindow(w), myWidget(new QTreeWidgetItem(parent, 2)), parent(nullptr),isExpanded(false) {
    myJob=j;
    myJob->saveParent(this);
    connect(this,SIGNAL(killInWindow(jobTreeItem*)),mainWindow,SLOT(removeJob(jobTreeItem*)));
    connect(this,SIGNAL(editJobPopup(jobTreeItem*)),mainWindow,SLOT(editJob(jobTreeItem*)));
}

jobTreeItem::jobTreeItem(Window* w, Job* j,QTreeWidgetItem * parent, const QStringList & strings)
    :mainWindow(w), myWidget(new QTreeWidgetItem(parent, strings, 2)), parent(nullptr),isExpanded(false) {
    myJob=j;
    myJob->saveParent(this);
    connect(this,SIGNAL(killInWindow(jobTreeItem*)),mainWindow,SLOT(removeJob(jobTreeItem*)));
    connect(this,SIGNAL(editJobPopup(jobTreeItem*)),mainWindow,SLOT(editJob(jobTreeItem*)));
}

jobTreeItem::jobTreeItem(Window* w, Job* j,QTreeWidgetItem * parent, QTreeWidgetItem * preceding)
    :mainWindow(w), myWidget(new QTreeWidgetItem(parent, preceding, 2)), parent(nullptr),isExpanded(false) {
    myJob=j;
    myJob->saveParent(this);
    connect(this,SIGNAL(killInWindow(jobTreeItem*)),mainWindow,SLOT(removeJob(jobTreeItem*)));
    connect(this,SIGNAL(editJobPopup(jobTreeItem*)),mainWindow,SLOT(editJob(jobTreeItem*)));
}

jobTreeItem::jobTreeItem(const jobTreeItem& t)
    :QObject(), mainWindow(t.mainWindow), myWidget(new QTreeWidgetItem(t.getWidget())), parent(nullptr),isExpanded(false) {
    myJob = t.getJob()->getCopy();
    myJob->saveParent(this);
    connect(this,SIGNAL(killInWindow(jobTreeItem*)),mainWindow,SLOT(removeJob(jobTreeItem*)));
    connect(this,SIGNAL(editJobPopup(jobTreeItem*)),mainWindow,SLOT(editJob(jobTreeItem*)));
}

void jobTreeItem::reconnect() {
    connect(this,SIGNAL(killInWindow(jobTreeItem*)),mainWindow,SLOT(removeJob(jobTreeItem*)));
    //connect(this,SIGNAL(editJobPopup(jobTreeItem*)),mainWindow,SLOT(editJob(jobTreeItem*)));
    connect(myWidget->treeWidget()->itemWidget(myWidget,3),SIGNAL(clicked()),this,SLOT(editJob()));
}

jobTreeItem::~jobTreeItem() {
    delete myJob;
    for (auto child : children) {
        delete child;
    }
}

void jobTreeItem::addMyWidget(QTreeWidget* tree) {
    QToolButton *editBox = new QToolButton(mainWindow);
    editBox->setText(QApplication::translate("MainWindow", "...", 0));
    dropMenu=new QMenu;
    editBox->setMenu(dropMenu);
    connect(editBox,SIGNAL(clicked(bool)),editBox,SLOT(showMenu()));
    QAction* action(dropMenu->addAction("Edit Job"));
    connect(action,SIGNAL(triggered()),this,SLOT(editJob()));
    if (myJob->isOverlord()) {
        action=dropMenu->addAction("Add sub job");
        connect(action,SIGNAL(triggered(bool)),this,SLOT(AddJobPopup()));
        action=dropMenu->addAction("Add sub job type");
        connect(action,SIGNAL(triggered(bool)),this,SLOT(AddOverlordPopup()));
        action=dropMenu->addAction("Convert to simple job");
        connect(action,SIGNAL(triggered(bool)),this,SLOT(toSimple()));
        if (myJob->hasFixedDeadline()) {
            action=dropMenu->addAction("View time management");
            connect(action,SIGNAL(triggered()),this,SLOT(timePopup()));
        }
    }
    else {
        action=dropMenu->addAction("Allow sub-jobs");
        connect(action,SIGNAL(triggered(bool)),this,SLOT(toOverlord()));
    }
    action=dropMenu->addAction("Delete job");
    connect(action,SIGNAL(triggered(bool)),this,SLOT(deleteDirect()));
    tree->setItemWidget(myWidget, 3, editBox);
}

void jobTreeItem::addWidget(QTreeWidget* tree) {
    myWidget = new QTreeWidgetItem(2);
    makeWidget();
    tree->addTopLevelItem(myWidget);
    addMyWidget(tree);
    for (auto child: children) {
        child->addWidget(tree,myWidget);
    }
    myWidget->setExpanded(isExpanded);
}

void jobTreeItem::addWidget(QTreeWidget* tree, QTreeWidgetItem* parent) {
    myWidget = new QTreeWidgetItem(2);
    makeWidget();
    parent->addChild(myWidget);
    addMyWidget(parent->treeWidget());
    for (auto child: children) {
        child->addWidget(tree,myWidget);
    }
    myWidget->setExpanded(isExpanded);
}

void jobTreeItem::rebuildMyMenu() {
    myMenu->clear();
    for (auto child: children) {
        child->addToMenuAgain(myMenu);
    }
    myMenu->addSeparator();
    QString completedJob(myJob->Name().c_str());
    completedJob.append(tr(" completed"));
    QAction* deleteButton (myMenu->addAction(completedJob));
    connect(deleteButton,SIGNAL(triggered()),this,SLOT(deleteMenu()));
}

void jobTreeItem::makeWidget() {
    myWidget->setText(2, QLocale().toString(myJob->due(), "dd MMMM yyyy h:mm ap"));
    switch(myJob->getPriority()) {
        case HIGH: myWidget->setText(1, QApplication::translate("MainWindow", "High", 0));
            break;
        case MEDIUM: myWidget->setText(1, QApplication::translate("MainWindow", "Medium", 0));
            break;
        case LOW: myWidget->setText(1, QApplication::translate("MainWindow", "Low", 0));
            break;
        case UNKNOWN: std::cerr << "shouldn't be unknown" << std::endl;
    }
    myWidget->setText(0, QApplication::translate("MainWindow", myJob->Name().c_str(), 0));
}

void jobTreeItem::recalculateMaxJobs() {
    jobOverlord* jobO(dynamic_cast<jobOverlord*>(myJob));
    jobO->recalculateMaxJobs();
    if (parent!=nullptr) {
        parent->recalculateMaxJobs();
    }
}

void jobTreeItem::addChild(jobTreeItem* child) {
    child->getJob()->setParent(dynamic_cast<jobOverlord*>(myJob));
    child->setParent(this);
    myWidget->addChild(child->getWidget());
    child->addMyWidget(myWidget->treeWidget());

    children.append(child);
    recalculateMaxJobs();
    qSort(children.begin(),children.end(),sortFunc);
    int it (children.indexOf(child));
    it++;
    child->addToMenu(myMenu);
    rebuildMyMenu();
    connect(child,SIGNAL(deleteSignal(jobTreeItem*)),this,SLOT(deleteFromMenu(jobTreeItem*)));
}

void jobTreeItem::addSomeonesChild(jobTreeItem* child) {
    child->getJob()->setParent(dynamic_cast<jobOverlord*>(myJob));
    child->setParent(this);
    myWidget->addChild(child->getWidget());

    children.append(child);
    recalculateMaxJobs();
    qSort(children.begin(),children.end(),sortFunc);
    int it (children.indexOf(child));
    it++;
    child->addToMenuAgain(myMenu);
    rebuildMyMenu();
    connect(child,SIGNAL(deleteSignal(jobTreeItem*)),this,SLOT(deleteFromMenu(jobTreeItem*)));
}

void jobTreeItem::removeChild(jobTreeItem* j) {
    children.removeOne(j);
    disconnect(j, 0, 0, 0);
    recalculateMaxJobs();
}

bool jobTreeItem::operator<(const jobTreeItem& t) const {
    return compareJobs(myJob,t.getJob());
}

void jobTreeItem::deleteMenu() {
    QMessageBox::StandardButton reply;
    std::string str("Mark "+myJob->Name()+" as done?");
    reply = QMessageBox::question(nullptr, "Job done", str.c_str(),
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (children.size()!=0) {
            std::vector<jobOverlord*> toDeleteToo(dynamic_cast<jobOverlord*>(myJob)->getJobTypes());
            for (jobOverlord* del : toDeleteToo) {
                del->getParent()->deleteSignal(del->getParent());
            }
        }
        deleteSignal(this);
    }
}

void jobTreeItem::deleteDirect() {
    if (children.size()!=0) {
        QMessageBox question;
        std::string str("Deleting this job will also delete " + std::to_string(children.size())+ " sub-jobs");
        question.setText(str.c_str());
        QPushButton* delButt(question.addButton(tr("Delete all"),QMessageBox::AcceptRole));
        QPushButton* dispButt(question.addButton(tr("Displace incomplete jobs"),QMessageBox::ActionRole));
        QPushButton* defaut (question.addButton(tr("Cancel"),QMessageBox::NoRole));
        question.setDefaultButton(defaut);
        question.setWindowTitle("");
        question.exec();
        if (question.clickedButton() == delButt) {
            if (children.size()!=0) {
                std::vector<jobOverlord*> toDeleteToo(dynamic_cast<jobOverlord*>(myJob)->getJobTypes());
                for (jobOverlord* del : toDeleteToo) {
                    del->getParent()->deleteSignal(del->getParent());
                }
            }
            deleteSignal(this);
        }
        else if (question.clickedButton() == dispButt) {
            displaceJobs();
            deleteSignal(this);
        }
    }
    else {
        deleteSignal(this);
    }
}

void jobTreeItem::deleteMenu(QTreeWidgetItem* item,int) {
    if (item==myWidget) {
        QMessageBox::StandardButton reply;
        std::string str("Mark "+myJob->Name()+" as done?");
        reply = QMessageBox::question(nullptr, "Job done", str.c_str(),
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            deleteSignal(this);
        }
    }
}

void jobTreeItem::deleteFromMenu(jobTreeItem* item) {
    myMenu->removeAction(item->getMenuItem());
    jobOverlord* Overlord (dynamic_cast<jobOverlord*>(myJob));
    Overlord->removeJob(item->getJob());
    children.removeOne(item);
    myWidget->removeChild(item->getWidget());
    killInWindow(item);
}

void jobTreeItem::addToMenu(QMenu* menu) {
    menuItem = myJob->addToMenu(menu);
    connect(mainWindow->getTree(),SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(deleteMenu(QTreeWidgetItem*,int)));
    // if !(NULL) => action not menu
    if (myJob->isOverlord()) {
        jobOverlord* Overlord (dynamic_cast<jobOverlord*>(myJob));
        myMenu = Overlord->getMenu();
        for (auto babe : Overlord->getJobs()) {
            jobTreeItem* manager(new jobTreeItem(mainWindow,babe));
            manager->setParent(this);

            myWidget->addChild(manager->getWidget());
            children.append(manager);
            qSort(children.begin(),children.end(),sortFunc);
            connect(manager,SIGNAL(deleteSignal(jobTreeItem*)),this,SLOT(deleteFromMenu(jobTreeItem*)));

            QTreeWidgetItem* item(manager->getWidget());
            item->setText(2, QLocale().toString(babe->due(), "dd MMMM yyyy h:mm ap"));
            switch(babe->getPriority()) {
                case HIGH: item->setText(1, QApplication::translate("MainWindow", "High", 0));
                    break;
                case MEDIUM: item->setText(1, QApplication::translate("MainWindow", "Medium", 0));
                    break;
                case LOW: item->setText(1, QApplication::translate("MainWindow", "Low", 0));
                    break;
                case UNKNOWN: std::cerr << "shouldn't be unknown" << std::endl;
            }
            item->setText(0, QApplication::translate("MainWindow", babe->Name().c_str(), 0));
            manager->addToMenu(myMenu);
        }
        qSort(children.begin(),children.end(),sortFunc);
        for (auto babe : children) {
            myWidget->addChild(babe->getWidget());
            babe->addMyWidget(myWidget->treeWidget());
        }
        myMenu->addSeparator();
        QString completedJob(myJob->Name().c_str());
        completedJob.append(tr(" completed"));
        QAction* deleteButton (myMenu->addAction(completedJob));
        connect(deleteButton,SIGNAL(triggered()),this,SLOT(deleteMenu()));
        menu->addMenu(myMenu);
    }
    connect(menuItem,SIGNAL(triggered()),this,SLOT(deleteMenu()));
}

void jobTreeItem::addToMenuAgain(QMenu* menu) {
    menuItem = myJob->addToMenu(menu);
    // if !(NULL) => action not menu
    if (myJob->isOverlord()) {
        jobOverlord* Overlord (dynamic_cast<jobOverlord*>(myJob));
        myMenu = Overlord->getMenu();
        qSort(children.begin(),children.end(),sortFunc);
        for (auto babe : children) {
            babe->addToMenuAgain(myMenu);
        }
        myMenu->addSeparator();
        QString completedJob(myJob->Name().c_str());
        completedJob.append(tr(" completed"));
        QAction* deleteButton (myMenu->addAction(completedJob));
        connect(deleteButton,SIGNAL(triggered()),this,SLOT(deleteMenu()));
        menu->addMenu(myMenu);
    }
    connect(menuItem,SIGNAL(triggered()),this,SLOT(deleteMenu()));
}

bool jobTreeItem::sortFunc(jobTreeItem* a,jobTreeItem* b) {
    return (*a)<(*b);
}

bool jobTreeItem::update() {
    bool changed(myJob->updatePriority());
    if (myJob->isOverlord()) {
        recalculateMaxJobs();
        qSort(children.begin(),children.end(),sortFunc);
    }
    return changed;
}

void jobTreeItem::editJob() {
    editJobPopup(this);
}

void jobTreeItem::saveExpansionState() {
    isExpanded=myWidget->isExpanded();
    for (auto babe: children) {
        babe->saveExpansionState();
    }
}

void jobTreeItem::setParent(jobTreeItem* p) {
    parent=p;
    if (p==nullptr) {
        myJob->setParent(nullptr);
    }
    else {
        myJob->setParent(dynamic_cast<jobOverlord*>(p->getJob()));
    }
}

void jobTreeItem::toOverlord() {
    jobOverlord* newJob;
    if (myJob->hasFixedDeadline()) {
        newJob = new deadlinedJobOverlord(*dynamic_cast<deadlinedJob*>(myJob));
    }
    else {
        newJob = new jobOverlord(*myJob);
    }
    jobOverlord* parentJob(myJob->getJobParent());
    if (parentJob!=nullptr) {
        parentJob->removeJob(myJob);
        parentJob->add_child(newJob);
    }
    delete myJob;
    myJob=newJob;
    myJob->saveParent(this);
    mainWindow->addOverlord(newJob);
    mainWindow->rebuild();
}

void jobTreeItem::toSimple() {
    if (children.size()>0) {
        QMessageBox question;
        std::string str("Modifying this job will delete " + std::to_string(children.size())+ " sub-jobs");
        question.setText(str.c_str());
        QPushButton* delButt(question.addButton(tr("Delete incomplete jobs"),QMessageBox::AcceptRole));
        QPushButton* dispButt(question.addButton(tr("Displace incomplete jobs"),QMessageBox::ActionRole));
        QPushButton* defaut (question.addButton(tr("Cancel"),QMessageBox::NoRole));
        question.setDefaultButton(defaut);
        question.setWindowTitle("");
        question.exec();
        if (question.clickedButton() == delButt) {
            if (children.size()!=0) {
                std::vector<jobOverlord*> toDeleteToo(dynamic_cast<jobOverlord*>(myJob)->getJobTypes());
                for (jobOverlord* del : toDeleteToo) {
                    del->getParent()->deleteSignal(del->getParent());
                }
            }
            for (auto child: children) {
                delete child;
            }
            Job* newJob;
            if (myJob->hasFixedDeadline()) {
                newJob = new deadlinedJob(*dynamic_cast<deadlinedJobOverlord*>(myJob));
            }
            else {
                newJob = new Job(*myJob);
            }
            mainWindow->extractOverlord(dynamic_cast<jobOverlord*>(myJob));
            jobOverlord* parentJob(myJob->getJobParent());
            if (parentJob!=nullptr) {
                parentJob->removeJob(myJob);
                parentJob->add_child(newJob);
            }
            delete myJob;
            myJob=newJob;
            myJob->saveParent(this);
            mainWindow->rebuild();
        }
        else if (question.clickedButton() == dispButt) {
            displaceJobs();
            Job* newJob;
            if (myJob->hasFixedDeadline()) {
                newJob = new deadlinedJob(*dynamic_cast<deadlinedJobOverlord*>(myJob));
            }
            else {
                newJob = new Job(*myJob);
            }
            mainWindow->extractOverlord(dynamic_cast<jobOverlord*>(myJob));
            jobOverlord* parentJob(myJob->getJobParent());
            if (parentJob!=nullptr) {
                parentJob->removeJob(myJob);
                parentJob->add_child(newJob);
            }
            delete myJob;
            myJob=newJob;
            myJob->saveParent(this);
            mainWindow->rebuild();
        }
    }
    else {
        Job* newJob;
        if (myJob->hasFixedDeadline()) {
            (*dynamic_cast<deadlinedJobOverlord*>(myJob));
        }
        else {
            newJob = new Job(*myJob);
        }
        mainWindow->extractOverlord(dynamic_cast<jobOverlord*>(myJob));
        jobOverlord* parentJob(myJob->getJobParent());
        if (parentJob!=nullptr) {
            parentJob->removeJob(myJob);
            parentJob->add_child(newJob);
        }
        delete myJob;
        myJob=newJob;
        myJob->saveParent(this);
        mainWindow->rebuild();
    }
}

void jobTreeItem::timePopup() {
    QDateTime now(QDateTime::currentDateTime());
    long secsPerJob(now.secsTo(myJob->due())*myJob->getWeight());
    int hours (secsPerJob/3600);
    int mins((secsPerJob-hours*3600)/60);
    int secs(secsPerJob-hours*3600-mins*60);
    std::string str("To complete this job on time, one job must be completed every\n");
    if (hours>1) {
        str+=std::to_string(hours);
        str+=" hours ";
    }
    else if (hours>0) {
        str+=std::to_string(hours);
        str+=" hour ";
    }
    if (mins>1) {
        str+=std::to_string(mins);
        str+=" mins ";
    }
    else if (mins>0) {
        str+=std::to_string(mins);
        str+=" min ";
    }
    if (secs>1) {
        str+=std::to_string(secs);
        str+=" secs\n";
    }
    else if (secs>0) {
        str+=std::to_string(secs);
        str+=" sec\n";
    }
    QMessageBox::information(mainWindow,myJob->Name().c_str(),str.c_str());
}

void jobTreeItem::displaceJobs() {
    while (children.size()!=0) {
        disconnect(children[0], 0, 0, 0);
        if (parent!=nullptr) {
            parent->addChild(children[0]);
        }
        else {
            mainWindow->addJob(children[0]);
        }
        children[0]->reconnect();
        children.removeFirst();
    }
}


void jobTreeItem::AddJobPopup() {
    mainWindow->AddJobPopUp(dynamic_cast<jobOverlord*>(myJob));
}

void jobTreeItem::AddOverlordPopup() {
    mainWindow->AddOverlordPopUp(dynamic_cast<jobOverlord*>(myJob));
}
