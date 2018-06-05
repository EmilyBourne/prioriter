#include "deadlinedjob.h"
#include "deadlinedjoboverlord.h"
#include <fstream>

deadlinedJob::deadlinedJob(Job const& j)
    :Job(j) {
    getPriorityChange();
}

deadlinedJob::deadlinedJob(Job* j)
    :Job(j) {
    getPriorityChange();
}

deadlinedJob::deadlinedJob(deadlinedJob* j)
    :Job(j), priorityChange(j->PriorityChange()) {}

deadlinedJob::deadlinedJob(jobOverlord* j)
    :Job(j) {
    getPriorityChange();
}

deadlinedJob::deadlinedJob(deadlinedJobOverlord* j)
    :Job(j), priorityChange(j->PriorityChange()) {}

deadlinedJob::deadlinedJob(std::string const& s)
    : Job(s) {
    int i(s.rfind(" "));
    int j(s.rfind(" ",i-1));
    name=name.substr(0,name.size()-s.size()+j);
    std::string pChange(s.substr(j+1,s.size()-j-1));
    i=pChange.find("/");
    int day=std::stoi(pChange.substr(0,i));
    j=pChange.find("/",i+1);
    int month=std::stoi(pChange.substr(i+1,j-i-1));
    i=pChange.find(" ",j);
    int year=std::stoi(pChange.substr(j+1,i-j-1));
    QDate d(year,month,day);
    j=pChange.find(":",i);
    int hour=std::stoi(pChange.substr(i+1,j-i-1));
    i=pChange.find(":",j+1);
    int minute=std::stoi(pChange.substr(j+1,i-j-1));
    QTime t(hour,minute);
    priorityChange=QDateTime(d,t);
}

deadlinedJob::deadlinedJob(std::string const& s,QDateTime const& d)
    : Job(s,d) {
    getPriorityChange();
}

deadlinedJob::deadlinedJob(std::string const& s,QDateTime const& d,Priority const& p)
    : Job(s,d,p) {
    getPriorityChange();
}

deadlinedJob::deadlinedJob(std::string const& s,const QDate& d)
    : Job(s,d) {
    getPriorityChange();
}

deadlinedJob::deadlinedJob(std::string const& s,const QTime& t)
    : Job(s,t) {
    getPriorityChange();
}

deadlinedJob::deadlinedJob(std::string const& s,const QDate& d,const QTime& t)
    : Job(s,d,t) {
    getPriorityChange();
}

deadlinedJob::deadlinedJob(std::string const& s,const QDate& d,Priority const& p)
    : Job(s,d,p) {
    getPriorityChange();
}

deadlinedJob::deadlinedJob(std::string const& s,const QTime& t,Priority const& p)
    : Job(s,t,p) {
    getPriorityChange();
}

deadlinedJob::deadlinedJob(std::string const& s,const QDate& d,const QTime& t,Priority const& p)
    : Job(s,d,t,p) {
    getPriorityChange();
}

bool deadlinedJob::updatePriority() {
    QDateTime today (QDateTime::currentDateTime());
    if (priorityChange<today) {
        switch (priorite) {
            case LOW: {
                priorite=MEDIUM;
                int secsLeft(today.secsTo(aRendre));
                priorityChange=priorityChange.addSecs(secsLeft/2);
                return true;
            }
            case MEDIUM: {
                priorite=HIGH;
                priorityChange=aRendre.addSecs(1);
                return true;
            }
            default: priorite=HIGH;
        }
    }
    return false;
}

void deadlinedJob::writeJob(const QString &s) const {
    std::ofstream writeFile;
    writeFile.open(s.toStdString(),std::ofstream::out | std::ofstream::app);
    writeFile << "d " << aRendre << " " << priorite << " " << name << " " << priorityChange << std::endl;
    writeFile.close();
}

void deadlinedJob::getPriorityChange() {
    if (priorite==MEDIUM) {
        QDateTime today (QDateTime::currentDateTime());
        int secsLeft(today.secsTo(aRendre));
        priorityChange=QDateTime::currentDateTime().addSecs(secsLeft/2);
    }
    else if (priorite==LOW) {
        QDateTime today (QDateTime::currentDateTime());
        int secsLeft(today.secsTo(aRendre));
        priorityChange=QDateTime::currentDateTime().addSecs(secsLeft/3);
    }
    else {
        priorityChange=aRendre.addSecs(1);
    }
}

Job* deadlinedJob::getCopy() const {
    return new deadlinedJob(*this);
}

bool deadlinedJob::DoLater(Job const& j) const {
    return do_sooner(j.due(),j.getPriority(),j.Name(),false,j.getWeight(),aRendre,priorite,name,true,weight);
}

bool deadlinedJob::DoLater(deadlinedJob const& j) const {
   return do_sooner(j.due(),j.getPriority(),j.Name(),true,j.getWeight(),aRendre,priorite,name,true,weight);
}

bool deadlinedJob::DoLater(jobOverlord const& j) const {
    if (j.getJobs().size()>0) {
        return !j.DoLater(*this);
    }
    else {
        return do_sooner(j.due(),j.getPriority(),j.Name(),false,j.getWeight(),aRendre,priorite,name,true,weight);
    }
}

bool deadlinedJob::DoLater(deadlinedJobOverlord const& j) const {
    if (j.getJobs().size()>0) {
        return !j.DoLater(*this);
    }
    else {
        return do_sooner(j.due(),j.getPriority(),j.Name(),true,j.getWeight(),aRendre,priorite,name,true,weight);
    }
}

bool deadlinedJob::DoLater(Job* j) const {
    return j->DoLater(*this);
}

deadlinedJob* deadlinedJob::makeJob(Job* j) {
    if (j->hasFixedDeadline()) {
        if (j->isOverlord()) {
            return new deadlinedJob(dynamic_cast<deadlinedJobOverlord*>(j));
        }
        else {
            return new deadlinedJob(dynamic_cast<deadlinedJob*>(j));
        }
    }
    else {
        if (j->isOverlord()) {
            return new deadlinedJob(dynamic_cast<jobOverlord*>(j));
        }
        else {
            return new deadlinedJob(j);
        }
    }
}

void deadlinedJob::setPriority(Priority p) {
    if (priorite!=p) {
        priorite=p;
        getPriorityChange();
    }
}

void deadlinedJob::setARendre(QDateTime const& d) {
    if (d!=aRendre) {
        aRendre=d;
        getPriorityChange();
    }
}
