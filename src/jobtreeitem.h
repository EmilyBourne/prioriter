#ifndef JOBTREEITEM_H
#define JOBTREEITEM_H
#include <QTreeWidgetItem>
#include <QMenu>
#include "job.h"

class Window;

class jobTreeItem : public QObject
{
    Q_OBJECT
public:
    jobTreeItem(Window*,Job*);
    jobTreeItem(Window*,Job*,const QStringList&);
    jobTreeItem(Window*,Job*,QTreeWidget*);
    jobTreeItem(Window*,Job*,QTreeWidget*, const QStringList&);
    jobTreeItem(Window*,Job*,QTreeWidget*, QTreeWidgetItem*);
    jobTreeItem(Window*,Job*,QTreeWidgetItem*);
    jobTreeItem(Window*,Job*,QTreeWidgetItem*, const QStringList&);
    jobTreeItem(Window*,Job*,QTreeWidgetItem*, QTreeWidgetItem*);
    jobTreeItem(const jobTreeItem&);
    ~jobTreeItem();

    virtual void addChild(jobTreeItem*);
    virtual void addSomeonesChild(jobTreeItem*);
    virtual void removeChild(jobTreeItem*);
    void setParent(jobTreeItem* p);
    jobTreeItem* getParent() {return parent; }

    void reconnect();
    void addToMenu(QMenu*);
    void addToMenuAgain(QMenu*);
    Job* getJob() const {return myJob; }
    Job** getSettableJob() {return &myJob; }
    QAction* getMenuItem() const {return menuItem; }
    QMenu* getMenu() const {return myMenu; }
    virtual bool operator<(const jobTreeItem&) const;
    bool update();
    void addMyWidget(QTreeWidget* tree);
    void addWidget(QTreeWidget* tree);
    void addWidget(QTreeWidget*,QTreeWidgetItem*);
    QTreeWidgetItem* getWidget() const {return myWidget; }
    static bool sortFunc(jobTreeItem*,jobTreeItem*);
    void saveExpansionState();
    void recalculateMaxJobs();
signals:
    void deleteSignal(jobTreeItem*);
    void killInWindow(jobTreeItem*);
    void died(jobTreeItem*);
    void editJobPopup(jobTreeItem*);
public slots:
    void deleteMenu();
    void deleteDirect();
    void deleteMenu(QTreeWidgetItem*,int);
    void AddJobPopup();
    void AddOverlordPopup();
private slots:
    void deleteFromMenu(jobTreeItem*);
    void editJob();
    void toOverlord();
    void toSimple();
    void timePopup();
private:
    void rebuildMyMenu();
    void makeWidget();
    void displaceJobs();
    Job* myJob;
    QMenu* myMenu;
    QMenu* dropMenu;
    QAction* menuItem;
    Window* mainWindow;
    QList<jobTreeItem*> children;
    QTreeWidgetItem* myWidget;
    jobTreeItem* parent;
    bool isExpanded;
};

#endif // JOBTREEITEM_H
