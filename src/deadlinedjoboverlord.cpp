#include "deadlinedjoboverlord.h"
#include <fstream>
#include <QDir>

deadlinedJobOverlord::deadlinedJobOverlord(Job const& j)
    :Job(j), deadlinedJob(j), jobOverlord(j) {}

deadlinedJobOverlord::deadlinedJobOverlord(deadlinedJobOverlord const& j)
    :Job(j), deadlinedJob(j), jobOverlord(j) {}

deadlinedJobOverlord::deadlinedJobOverlord(jobOverlord const& j)
    :Job(j), deadlinedJob(j), jobOverlord(j) {}

deadlinedJobOverlord::deadlinedJobOverlord(deadlinedJob const& j)
    :Job(j), deadlinedJob(j), jobOverlord(j) {}


deadlinedJobOverlord::deadlinedJobOverlord(Job* j)
    :Job(j), deadlinedJob(j), jobOverlord(j) {}

deadlinedJobOverlord::deadlinedJobOverlord(jobOverlord* j)
    :Job(j), deadlinedJob(j), jobOverlord(j) {}

deadlinedJobOverlord::deadlinedJobOverlord(deadlinedJob* j)
    :Job(j), deadlinedJob(j), jobOverlord(j) {}

deadlinedJobOverlord::deadlinedJobOverlord(deadlinedJobOverlord* j) 
    :Job(j), deadlinedJob(j), jobOverlord(j) {}

deadlinedJobOverlord::deadlinedJobOverlord(std::string const& s)
    : Job(s), deadlinedJob(s) {
    
    QDir MainFolder(QDir::homePath()+"/.prioriter/savedJobs");
    std::ifstream babes;
    folderName=name;
    babes.open(QDir::toNativeSeparators(MainFolder.absoluteFilePath((folderName+".txt").c_str())).toStdString(),std::ofstream::in);
    if (babes.is_open()) {
        std::string child;
        std::getline(babes,name);
        while (!babes.eof()) {
            std::getline(babes,child);
            if (child.size()>2) {
                switch (child[0]) {
                    case 'j': babies.push_back(new Job(child.substr(2,child.size()-2))); break;
                    case 'd': babies.push_back(new deadlinedJob(child.substr(2,child.size()-2))); break;
                    case 'o': babies.push_back(new jobOverlord(child.substr(2,child.size()-2))); break;
                    case 'p': babies.push_back(new deadlinedJobOverlord(child.substr(2,child.size()-2))); break;
                    default: std::cerr << "unrecognised job type : " << child[0] << std::endl;
                }
                babies.back()->setParent(this);
            }
        }
        std::sort(babies.begin(),babies.end(),compareJobs);
        babes.close();
    }
    recalculateMaxJobs();
    if (maxJobs==0) weight=1;
    else weight=std::min(1.0/maxJobs,1.0);
}

deadlinedJobOverlord::deadlinedJobOverlord(std::string const& s,QDateTime const& d)
    : deadlinedJob(s,d) {}

deadlinedJobOverlord::deadlinedJobOverlord(std::string const& s,QDateTime const& d,Priority const& p)
    : Job(s,d,p), deadlinedJob(s,d,p) {}

deadlinedJobOverlord::deadlinedJobOverlord(std::string const& s,const QDate& d)
    : Job(s,d), deadlinedJob(s,d) {}

deadlinedJobOverlord::deadlinedJobOverlord(std::string const& s,const QTime& t)
    : Job(s,t), deadlinedJob(s,t) {}

deadlinedJobOverlord::deadlinedJobOverlord(std::string const& s,const QDate& d,const QTime& t)
    : Job(s,d,t), deadlinedJob(s,d,t) {}

deadlinedJobOverlord::deadlinedJobOverlord(std::string const& s,const QDate& d,Priority const& p)
    : Job(s,d,p), deadlinedJob(s,d,p) {}

deadlinedJobOverlord::deadlinedJobOverlord(std::string const& s,const QTime& t,Priority const& p)
    : Job(s,t,p), deadlinedJob(s,t,p) {}

deadlinedJobOverlord::deadlinedJobOverlord(std::string const& s,const QDate& d,const QTime& t,Priority const& p)
    : Job(s,d,t,p), deadlinedJob(s,d,t,p) {}

deadlinedJobOverlord::~deadlinedJobOverlord() {}

bool deadlinedJobOverlord::updatePriority() {
    return deadlinedJob::updatePriority() || jobOverlord::updatePriority();
}

QAction* deadlinedJobOverlord::addToMenu(QMenu* q) {
    return jobOverlord::addToMenu(q);
}

/*
QAction* deadlinedJobOverlord::addToMenu(QMenu* menu,QAction* before) {
    return jobOverlord::addToMenu(menu,before);
}*/

void deadlinedJobOverlord::writeJob(QString const& s) const {
    std::ofstream writeFile;
    writeFile.open(s.toStdString(),std::ofstream::out | std::ofstream::app);
    writeFile << "p " << aRendre << " " << priorite << " " << folderName << " " << priorityChange << std::endl;
    writeFile.close();
    QDir MainFolder (s.left(QDir::fromNativeSeparators(s).lastIndexOf("/")));
    QString filename (QDir::toNativeSeparators(MainFolder.absoluteFilePath((folderName+".txt").c_str())));
    writeFile.open(filename.toStdString(),std::ofstream::out);
    writeFile << name << std::endl;
    writeFile.close();
    for (size_t i(0); i<babies.size(); ++i) {
        babies[i]->writeJob(filename);
    }
}

Job* deadlinedJobOverlord::getCopy() const {
    return new deadlinedJobOverlord(*this);
}

bool deadlinedJobOverlord::DoLater(Job const& j) const {
    if (babies.size()>0) {
        bool subJobImportanceTooLow(Job::DoLater(babies[0]));
        //std::cout << "subjob importance " << subJobImportanceTooLow << std::endl;
        if (subJobImportanceTooLow) {
            return do_sooner(j.due(),j.getPriority(),j.Name(),false,j.getWeight(),aRendre,priorite,name,true,weight);
        }
        else return babies[0]->DoLater(j);
    }
    else {
        return do_sooner(j.due(),j.getPriority(),j.Name(),false,j.getWeight(),aRendre,priorite,name,true,weight);
    }
}

bool deadlinedJobOverlord::DoLater(deadlinedJob const& j) const {
    if (babies.size()>0) {
        bool subJobImportanceTooLow(Job::DoLater(babies[0]));
        //std::cout << "subjob importance " << subJobImportanceTooLow << std::endl;
        if (subJobImportanceTooLow) {
            return do_sooner(j.due(),j.getPriority(),j.Name(),true,j.getWeight(),aRendre,priorite,name,true,weight);
        }
        else return babies[0]->DoLater(j);
    }
    else {
        return do_sooner(j.due(),j.getPriority(),j.Name(),true,j.getWeight(),aRendre,priorite,name,true,weight);
    }
}

bool deadlinedJobOverlord::DoLater(jobOverlord const& j) const {
    if (babies.size()>0) {
        bool subJobImportanceTooLow(deadlinedJob::DoLater(babies[0]));
        if (subJobImportanceTooLow) return deadlinedJob::DoLater(j);
        else return babies[0]->DoLater(j);
    }
    else {
        return deadlinedJob::DoLater(j);
    }
}


bool deadlinedJobOverlord::DoLater(deadlinedJobOverlord const& j) const {
    if (babies.size()>0) {
        bool subJobImportanceTooLow(deadlinedJob::DoLater(babies[0]));
        if (subJobImportanceTooLow) return deadlinedJob::DoLater(j);
        else return babies[0]->DoLater(j);
    }
    else {
        return deadlinedJob::DoLater(j);
    }
}

bool deadlinedJobOverlord::DoLater(Job* j) const {
    return j->DoLater(*this);
}

deadlinedJobOverlord* deadlinedJobOverlord::makeJob(Job* j) {
    if (j->hasFixedDeadline()) {
        if (j->isOverlord()) {
            return new deadlinedJobOverlord(dynamic_cast<deadlinedJobOverlord*>(j));
        }
        else {
            return new deadlinedJobOverlord(dynamic_cast<deadlinedJob*>(j));
        }
    }
    else {
        if (j->isOverlord()) {
            return new deadlinedJobOverlord(dynamic_cast<jobOverlord*>(j));
        }
        else {
            return new deadlinedJobOverlord(j);
        }
    }
}
