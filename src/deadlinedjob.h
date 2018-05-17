#ifndef DEADLINEDJOB_H
#define DEADLINEDJOB_H
#include "job.h"

class deadlinedJob : public virtual Job
{
public:
    deadlinedJob(Job const& j);
    deadlinedJob(Job*);
    deadlinedJob(deadlinedJob*);
    deadlinedJob(jobOverlord*);
    deadlinedJob(deadlinedJobOverlord*);
    deadlinedJob(std::string const&);
    deadlinedJob(std::string const&,QDateTime const& d);
    deadlinedJob(std::string const&,QDateTime const&,Priority const&);
    deadlinedJob(std::string const&,const QDate&);
    deadlinedJob(std::string const&,const QTime&);
    deadlinedJob(std::string const&,const QDate&,const QTime&);
    deadlinedJob(std::string const&,const QDate&,Priority const&);
    deadlinedJob(std::string const&,const QTime&,Priority const&);
    deadlinedJob(std::string const&,const QDate&,const QTime&,Priority const&);
    virtual Priority getMaxPriority() const {return priorite; }
    virtual bool updatePriority();
    virtual bool DoLater(Job*) const;
    virtual bool DoLater(Job const&) const;
    virtual bool DoLater(jobOverlord const&) const;
    virtual bool DoLater(deadlinedJob const&) const;
    virtual bool DoLater(deadlinedJobOverlord const&) const;
    virtual void writeJob(QString const&) const;
    virtual Job* getCopy() const;
    virtual bool isOverlord () const {return false; }
    virtual bool hasFixedDeadline () const {return true; }
    QDateTime const& PriorityChange() const {return priorityChange; }
    static deadlinedJob* makeJob(Job* j);
    virtual void setPriority(Priority p);
    virtual void setARendre(QDateTime const& d);
protected:
    void getPriorityChange();
    QDateTime priorityChange;
};

#endif // DEADLINEDJOB_H
