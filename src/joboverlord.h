#ifndef JOBOVERLORD_H
#define JOBOVERLORD_H
#include "job.h"
#include <vector>
#include <utility>
#include <QMenu>

class jobOverlord : public virtual Job
{
public:
    jobOverlord(Job const&);
    jobOverlord(jobOverlord const&);
    jobOverlord(Job*);
    jobOverlord(jobOverlord*);
    jobOverlord(std::string const&);
    jobOverlord(std::string const&,QDateTime const& d);
    jobOverlord(std::string const&,QDateTime const&,Priority const&);
    jobOverlord(std::string const&,const QDate&);
    jobOverlord(std::string const&,const QTime&);
    jobOverlord(std::string const&,const QDate&,const QTime&);
    jobOverlord(std::string const&,const QDate&,Priority const&);
    jobOverlord(std::string const&,const QTime&,Priority const&);
    jobOverlord(std::string const&,const QDate&,const QTime&,Priority const&);
    void add_child(std::string const&);
    void add_child(std::string const&,QDateTime const& d);
    void add_child(std::string const&,QDateTime const&,Priority const&);
    void add_child(std::string const&,const QDate&);
    void add_child(std::string const&,const QTime&);
    void add_child(std::string const&,const QDate&,const QTime&);
    void add_child(std::string const&,const QDate&,Priority const&);
    void add_child(std::string const&,const QTime&,Priority const&);
    void add_child(std::string const&,const QDate&,const QTime&,Priority const&);
    void add_child(Job*const& j);
    void add_deadlined_child(std::string const&);
    void add_deadlined_child(std::string const&,QDateTime const& d);
    void add_deadlined_child(std::string const&,QDateTime const&,Priority const&);
    void add_deadlined_child(std::string const&,const QDate&);
    void add_deadlined_child(std::string const&,const QTime&);
    void add_deadlined_child(std::string const&,const QDate&,const QTime&);
    void add_deadlined_child(std::string const&,const QDate&,Priority const&);
    void add_deadlined_child(std::string const&,const QTime&,Priority const&);
    void add_deadlined_child(std::string const&,const QDate&,const QTime&,Priority const&);
    void add_deadlined_child(deadlinedJob*const& j);
    virtual bool updatePriority();
    virtual void writeJob(QString const&) const;
    std::vector<Job*> const& getJobs() const {return babies; }
    std::vector<jobOverlord*> getJobTypes() const;
    virtual Priority getMaxPriority() const;
    ~jobOverlord();
    virtual QAction* addToMenu(QMenu*);
    //virtual QAction* addToMenu(QMenu*,QAction*);
    virtual bool isOverlord () const {return true; }
    virtual bool hasFixedDeadline () const {return false; }
    virtual Job* getCopy() const;
    QMenu* getMenu() const {return myMenu; }
    void removeJob(int jAddress);
    void removeJob(Job* jAddress);
    virtual bool DoLater(Job*) const;
    virtual bool DoLater(Job const&) const;
    virtual bool DoLater(deadlinedJob const&) const;
    virtual bool DoLater(jobOverlord const&) const;
    virtual bool DoLater(deadlinedJobOverlord const&) const;
    void complete() const;
    virtual int jobSize() const;
    void recalculateMaxJobs();
    std::string BigName() const;
    std::string const& getFolderName() const {return folderName; }
    static jobOverlord* makeJob(Job* j);
protected:
    void checkFolderName();
    jobOverlord();
    std::vector<Job*> babies;
    std::vector<std::pair<int,Job*>> deleteHelper;
    QMenu* myMenu;
    int maxJobs;
    std::string folderName;
};

#endif // JOBOVERLORD_H
