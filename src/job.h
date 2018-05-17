#ifndef JOB_H
#define JOB_H
#include <string>
#include <iostream>
#include <QMenu>
//#include <QVariant>
#include <QDateTime>

std::ostream& operator<<(std::ostream&, QDateTime const&);

class jobTreeItem;

enum Priority {
    HIGH = 2,
    MEDIUM = 1,
    LOW = 0,
    UNKNOWN = -1
};

class deadlinedJob;
class jobOverlord;
class deadlinedJobOverlord;

class Job {
public:
    Job(std::string const&);
    Job(std::string const&,QDateTime const& d);
    Job(std::string const&,QDateTime const&,Priority const&);
    Job(std::string const&,const QDate&);
    Job(std::string const&,const QTime&);
    Job(std::string const&,const QDate&,const QTime&);
    Job(std::string const&,const QDate&,Priority const&);
    Job(std::string const&,const QTime&,Priority const&);
    Job(std::string const&,const QDate&,const QTime&,Priority const&);
    Job(Job const&);
    Job(Job*);
    virtual ~Job();
    virtual bool updatePriority();
    virtual void writeJob(QString const&) const;
    virtual bool DoLater(Job const&) const;
    virtual bool DoLater(deadlinedJob const&) const;
    virtual bool DoLater(jobOverlord const&) const;
    virtual bool DoLater(deadlinedJobOverlord const&) const;
    virtual bool DoLater(Job*) const;
    const QDateTime& due() const {return aRendre; }
    std::string const& Name () const {return name; }
    Priority getPriority() const {return priorite; }
    virtual Priority getMaxPriority() const {return priorite; }
    virtual QAction* addToMenu(QMenu*);
    //virtual QAction* addToMenu(QMenu*,QAction*);
    virtual bool isOverlord () const {return false; }
    virtual bool hasFixedDeadline () const {return false; }
    virtual Job* getCopy() const;
    jobTreeItem* getParent() const {return dad; }
    void saveParent(jobTreeItem* p) {dad=p; }
    jobOverlord* getJobParent() const {return parent; }
    virtual void setARendre(QDateTime const& d) {aRendre=d; }
    virtual void setPriority(Priority p) {priorite=p; }
    std::string& setName() {return name; }
    double getWeight() const {return weight; }
    virtual int jobSize() const {return 1; }
    void setParent(jobOverlord* j);
    virtual std::string BigName() const {return name; }
    static Job* makeJob(Job* j);
protected:
    Job() {}
    QDateTime aRendre;
    Priority priorite;
    std::string name;
    jobTreeItem* dad;
    double weight;
    jobOverlord* parent;
};

Q_DECLARE_METATYPE(Job*)

bool compareJobs(Job*,Job*);

bool do_sooner(QDateTime const& date1, Priority priority1, std::string const& name1, bool dl1, double weight1,
              QDateTime const& date2, Priority priority2, std::string const& name2, bool dl2, double weight2,
              double comparisonFactor = 1.0/3.0);

bool compare(std::string const&,std::string const&);

#endif // JOB_H

