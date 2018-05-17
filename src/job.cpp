#include "job.h"
#include <fstream>
#include <iostream>
#include "joboverlord.h"
#include "deadlinedjoboverlord.h"
#include <cassert>

std::ostream& operator<<(std::ostream& writeFile, QDateTime const& dt) {
    writeFile << dt.date().day() << "/" << dt.date().month();
    writeFile << "/" << dt.date().year() << " " << dt.time().hour();
    writeFile << ":" << dt.time().minute() << ":" << dt.time().second();
    return writeFile;
}

Job::Job(Job const& j)
    :aRendre(j.aRendre), priorite(j.priorite), name(j.name), dad(nullptr), weight(j.weight), parent(nullptr) {}

Job::Job(Job* j)
    :aRendre(j->due()), priorite(j->getPriority()), name(j->Name()), dad(j->getParent()), weight(j->getWeight()), parent(j->getJobParent()) {}

Job::Job(std::string const& s)
    :dad(nullptr), weight(1.0), parent(nullptr) {
    int i(s.find("/"));
    std::string temp(s);
    int day=std::stoi(temp.substr(0,i));
    temp=temp.substr(i+1);
    i = temp.find("/");
    int month=std::stoi(temp.substr(0,i));
    temp=temp.substr(i+1);
    i = temp.find(" ");
    int year=std::stoi(temp.substr(0,i));
    temp=temp.substr(i+1);
    QDate d(year,month,day);
    i = temp.find(":");
    int hour=std::stoi(temp.substr(0,i));
    temp=temp.substr(i+1);
    i = temp.find(" ");
    int minute=std::stoi(temp.substr(0,i));
    temp=temp.substr(i+1);
    QTime t(hour,minute);
    aRendre=QDateTime(d,t);
    i = temp.find(" ");
    priorite=Priority(std::stoi(temp.substr(0,i)));
    name=temp.substr(i+1);
}

Job::Job(std::string const& s,QDateTime const& d)
    : aRendre(d), priorite(MEDIUM), name(s), dad(nullptr), weight(1.0), parent(nullptr) {}

Job::Job(std::string const& s,QDateTime const& d,Priority const& p)
    : aRendre(d), priorite(p), name(s), dad(nullptr), weight(1.0), parent(nullptr) {}

Job::Job(std::string const& s,const QDate& d)
    : aRendre(d), priorite(MEDIUM), name(s), dad(nullptr), weight(1.0), parent(nullptr) {}

Job::Job(std::string const& s,const QTime& t)
    : aRendre(QDate::currentDate(),t), priorite(MEDIUM), name(s), dad(nullptr), weight(1.0), parent(nullptr) {}

Job::Job(std::string const& s,const QDate& d,const QTime& t)
    : aRendre(d,t), priorite(MEDIUM), name(s), dad(nullptr), weight(1.0), parent(nullptr) {}

Job::Job(std::string const& s,const QDate& d,Priority const& p)
    : aRendre(d), priorite(p), name(s), dad(nullptr), weight(1.0), parent(nullptr) {}

Job::Job(std::string const& s,const QTime& t,Priority const& p)
    : aRendre(QDate::currentDate(),t), priorite(p), name(s), dad(nullptr), weight(1.0), parent(nullptr) {}

Job::Job(std::string const& s,const QDate& d,const QTime& t,Priority const& p)
    : aRendre(d,t), priorite(p), name(s), dad(nullptr), weight(1.0), parent(nullptr) {}

Job::~Job() {}

bool Job::updatePriority() {
    return false;
}

bool Job::DoLater(Job const& j) const {
    return do_sooner(j.aRendre,j.priorite,j.name,false,j.weight,aRendre,priorite,name,false,weight);
}

bool Job::DoLater(deadlinedJob const& j) const {
    return do_sooner(j.aRendre,j.priorite,j.name,true,j.weight,aRendre,priorite,name,false,weight);
}

bool Job::DoLater(jobOverlord const& j) const {
    if (j.getJobs().size()>0) {
        return !j.DoLater(*this);
    }
    else {
        return do_sooner(j.aRendre,j.priorite,j.name,false,j.weight,aRendre,priorite,name,false,weight);
    }
}

bool Job::DoLater(deadlinedJobOverlord const& j) const {
    if (j.getJobs().size()>0) {
        return !j.DoLater(*this);
    }
    else {
        return do_sooner(j.aRendre,j.priorite,j.name,true,weight,aRendre,priorite,name,false,weight);
    }
}

bool Job::DoLater(Job* j) const {
    return j->DoLater(*this);
}

void Job::writeJob(const QString &s) const {
    std::ofstream writeFile;
    writeFile.open(s.toStdString(),std::ofstream::out | std::ofstream::app);
    writeFile << "j " << aRendre << " " << priorite <<  " " << name << std::endl;
    writeFile.close();
}

QAction* Job::addToMenu(QMenu* menu) {
    return menu->addAction(name.c_str());
}

Job* Job::getCopy() const {
    return new Job(*this);
}

bool compareJobs(Job* a,Job* b) {
    return a->DoLater(b);
}

bool do_sooner(QDateTime const& date1,Priority priority1, std::string const& name1,bool dl1, double weight1,
              QDateTime const& date2,Priority priority2, std::string const& name2,bool dl2, double weight2,
               double comparisonFactor) {
    //std::cout << "testing " << date1.toString().toStdString();
    //std::cout << " " << priority1 << " " << name1 << " " << dl1 << " " << weight1 << std::endl;
    //std::cout << "against " << date2.toString().toStdString() << " " << priority2 << " " << name2 << " " << dl2 << " " << weight2 << std::endl;
    QDateTime now(QDateTime::currentDateTime());
    int to1(now.secsTo(date1));
    int to2(now.secsTo(date2));
    bool dueToday1(to1>0 && to1<43200);
    bool dueToday2(to2>0 && to2<43200);
    if (dl1 && dueToday1 && !dueToday2) {
        //std::cout << 1 << std::endl;
        return true;
    }
    if (dl2 && dueToday2 && !dueToday1  ) {
        //std::cout << 0 << std::endl;
        return false;
    }
    if (priority1==priority2) {
        if (date1.date()==date2.date()) {
            if (weight1!=weight2) {
                return weight1<weight2;
            }
            if (dl1==dl2) {
                //std::cout << (name1<name2) << std::endl;
                return compare(name1,name2);
            }
            else if (dl1) {
                //std::cout << 1 << std::endl;
                return true;
            }
            else {
                //std::cout << 0 << std::endl;
                return false;
            }
        }
        else {
            //std::cout << "date compare " << (date1<date2) << std::endl;
            return to1*weight1<to2*weight2;
        }
    }
    if (priority1==LOW) {
        //std::cout << 0 << std::endl;
        return false;
    }
    if (priority2==LOW) {
        //std::cout << 1 << std::endl;
        return true;
    }
    //medium/high
    to1*=weight1;
    to2*=weight2;
    bool handInFirst(to1<to2);
    if (priority1==HIGH && (handInFirst || to1==to2)) {
        //std::cout << 1 << std::endl;
        return true;
    }
    if (priority2==HIGH && (!handInFirst || to1==to2)) {
        //std::cout << 0 << std::endl;
        return false;
    }
    if (handInFirst) {
        //std::cout << (to1*comparisonFactor<=to2) << std::endl;
        return to1*comparisonFactor<=to2;
    }
    else {
        //std::cout << (to2*comparisonFactor>to1) << std::endl;
        return to2*comparisonFactor>to1;
    }
}

bool compare(std::string const& str1,std::string const& str2) {
    size_t i1(0);
    size_t i2(0);
    while (i1!=str1.size() && i2!=str2.size()) {
        char c1(str1[i1]);
        char c2(str2[i2]);
        if (c1<='9' && c1>='0' && c2<='9' && c2>='0') {
            char num1[20]("");
            char num2[20]("");
            int n1(0);
            int n2(0);
            while (c1<='9' && c1>='0' && i1!=str1.size()) {
                num1[n1++]=c1;
                c1=str1[++i1];
            }
            num1[n1]='\0';
            while (c2<='9' && c2>='0' && i2!=str2.size()) {
                num2[n2++]=c2;
                c2=str2[++i2];
            }
            num2[n2]='\0';
            float f1(atof(num1));
            float f2(atof(num2));
            if (f1!=f2) return f1<f2;
        }
        else if (c1!=c2) return c1<c2;
        else {
            i1++;
            i2++;
        }
    }
    if (i1!=str1.size()) return false;
    else if (i2!=str2.size()) return true;
    else return true;
}

Job* Job::makeJob(Job* j) {
    if (j->hasFixedDeadline()) {
        if (j->isOverlord()) {
            return new Job(dynamic_cast<deadlinedJobOverlord*>(j));
        }
        else {
            return new Job(dynamic_cast<deadlinedJob*>(j));
        }
    }
    else {
        if (j->isOverlord()) {
            return new Job(dynamic_cast<jobOverlord*>(j));
        }
        else {
            return new Job(j);
        }
    }
}

void Job::setParent(jobOverlord* j) {
    assert(j!=this);
    parent=j;
}
