#ifndef DEADLINEDdeadlinedJobOverlord_H
#define DEADLINEDdeadlinedJobOverlord_H
#include "deadlinedjob.h"
#include "joboverlord.h"
#include <vector>

class deadlinedJobOverlord : public deadlinedJob, public jobOverlord
{
public:
    deadlinedJobOverlord(const Job&);
    deadlinedJobOverlord(const jobOverlord&);
    deadlinedJobOverlord(const deadlinedJob&);
    deadlinedJobOverlord(const deadlinedJobOverlord&);
    deadlinedJobOverlord(Job* j);
    deadlinedJobOverlord(jobOverlord* j);
    deadlinedJobOverlord(deadlinedJob* j);
    deadlinedJobOverlord(deadlinedJobOverlord* j);
    deadlinedJobOverlord(std::string const&);
    deadlinedJobOverlord(std::string const&,QDateTime const& d);
    deadlinedJobOverlord(std::string const&,QDateTime const&,Priority const&);
    deadlinedJobOverlord(std::string const&,const QDate&);
    deadlinedJobOverlord(std::string const&,const QTime&);
    deadlinedJobOverlord(std::string const&,const QDate&,const QTime&);
    deadlinedJobOverlord(std::string const&,const QDate&,Priority const&);
    deadlinedJobOverlord(std::string const&,const QTime&,Priority const&);
    deadlinedJobOverlord(std::string const&,const QDate&,const QTime&,Priority const&);
    virtual Priority getMaxPriority() const {return jobOverlord::getMaxPriority(); }
    virtual bool updatePriority();
    virtual void writeJob(const QString &) const;
    virtual bool isOverlord () const {return true; }
    virtual bool hasFixedDeadline () const {return true; }
    ~deadlinedJobOverlord();
    virtual QAction* addToMenu(QMenu*);
    //virtual QAction* addToMenu(QMenu*,QAction*);
    virtual Job* getCopy() const;
    virtual bool DoLater(Job*) const;
    virtual bool DoLater(Job const&) const;
    virtual bool DoLater(deadlinedJob const&) const;
    virtual bool DoLater(jobOverlord const&) const;
    virtual bool DoLater(deadlinedJobOverlord const&) const;
    virtual int jobSize() const {return jobOverlord::jobSize(); }
    virtual std::string BigName() const {return jobOverlord::BigName(); }
    static deadlinedJobOverlord* makeJob(Job* j);
    virtual void setPriority(Priority p) {deadlinedJob::setPriority(p);}
    virtual void setARendre(QDateTime const& d) {deadlinedJob::setARendre(d); }
};


#endif // DEADLINEDdeadlinedJobOverlord_H
