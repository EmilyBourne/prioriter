#ifndef EDITJOB_H
#define EDITJOB_H
#include "ui_addjob.h"
#include "jobtreeitem.h"
#include <map>
class Window;

class EditJob: public QDialog, protected Ui::addJob
{
    Q_OBJECT
public:
    EditJob(Window*, std::vector<jobOverlord*> &overlords, jobTreeItem*);
    virtual ~EditJob();
private slots:
    void modifyJob();
private:
    Window* window;
    Job* jobPtr;
    jobTreeItem* manager;
    jobOverlord* daddy;
    int oldIndex;
    std::vector<jobOverlord*> &Overlords;
    std::map<int,int> dropdownToOverlordMapper;
};

#endif // EDITJOB_H
