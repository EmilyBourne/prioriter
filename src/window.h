#ifndef WINDOW_H
#define WINDOW_H
#include <QSystemTrayIcon>
#include <ui_mainwindow.h>
#include <vector>
#include "jobtreeitem.h"
#include <QSignalMapper>
#include <utility>
#include <QDir>

class Window : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
    Window();
    ~Window();

    void setVisible(bool visible);
    QTreeWidget* getTree () {return treeWidget; }

    void addJob(jobTreeItem*);
    void extractJob(jobTreeItem* manager);
    void addOverlord(jobOverlord*);
    void extractOverlord(jobOverlord*);

public slots:
    void rebuild();
    void AddJobPopUp(jobOverlord * = nullptr);
    void AddOverlordPopUp(jobOverlord* index = nullptr);

protected:
    void closeEvent(QCloseEvent *event);
#if defined(Q_WS_X11)
    bool eventFilter(QObject *object, QEvent *event);
#endif

private slots:
    void removeJob(jobTreeItem*);
    void QuitAndSave();
    void openWindow();
    void update();
    void editJob(jobTreeItem*);

private:
    void createTrayIcon(Priority jobPriorities, long nearestDeadline);
    void createActions();
    void createMenu();
    void rebuildTrayIconMenu();
    void rebuildTree();
    void Save();

    void setupProgram(QDir const&);

    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
    QAction* quitAction;
    QAction* showWindow;
    QSignalMapper* signalMapper;
    QList<jobTreeItem*> jobs;
    //std::vector<QAction*> menuItems;
    //std::vector<Job*> jobList;
    std::vector<jobOverlord*> overlordList;
    //std::vector<QTreeWidgetItem*> listItems;
    //std::vector<std::pair<int,Job*>> deleteHelper;
    int maxInt;
    QTimer* timer;
};

#endif // WINDOW_H
