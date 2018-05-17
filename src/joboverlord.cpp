#include "joboverlord.h"
#include "deadlinedjob.h"
#include <fstream>
#include <algorithm>
#include "deadlinedjoboverlord.h"
#include <stdio.h>
#include <QDir>

jobOverlord::jobOverlord(Job const& j)
    :Job(j), myMenu(nullptr), maxJobs(0) {
    folderName=name;
    checkFolderName();
}

jobOverlord::jobOverlord(jobOverlord const& j)
    :Job(j), myMenu(nullptr), folderName(j.folderName) {
    for (size_t i(0); i<j.babies.size(); ++i) {
        babies.push_back(j.babies[i]->getCopy());
    }
    recalculateMaxJobs();
}

jobOverlord::jobOverlord(Job* j)
    :Job(*j), myMenu(nullptr), maxJobs(0) {
    folderName=name;
    checkFolderName();
}

jobOverlord::jobOverlord(jobOverlord* j)
    :Job(*j), myMenu(nullptr), folderName(j->getFolderName()) {
    for (auto babe: j->getJobs()) {
        babies.push_back(babe);
        babe->setParent(this);
    }
    recalculateMaxJobs();
}

jobOverlord::jobOverlord(std::string const& s)
    : Job(s), myMenu(nullptr) {
    
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
}

jobOverlord::jobOverlord(std::string const& s,QDateTime const& d)
    : Job(s,d), myMenu(nullptr), maxJobs(0) {
    folderName=name;
    checkFolderName();
}

jobOverlord::jobOverlord(std::string const& s,QDateTime const& d,Priority const& p)
    : Job(s,d,p), myMenu(nullptr), maxJobs(0) {
    folderName=name;
    checkFolderName();
}

jobOverlord::jobOverlord(std::string const& s,const QDate& d)
    : Job(s,d), myMenu(nullptr), maxJobs(0) {
    folderName=name;
    checkFolderName();
}

jobOverlord::jobOverlord(std::string const& s,const QTime& t)
    : Job(s,t), myMenu(nullptr), maxJobs(0) {
    folderName=name;
    checkFolderName();
}

jobOverlord::jobOverlord(std::string const& s,const QDate& d,const QTime& t)
    : Job(s,d,t), myMenu(nullptr), maxJobs(0) {
    folderName=name;
    checkFolderName();
}

jobOverlord::jobOverlord(std::string const& s,const QDate& d,Priority const& p)
    : Job(s,d,p), myMenu(nullptr), maxJobs(0) {
    folderName=name;
    checkFolderName();
}

jobOverlord::jobOverlord(std::string const& s,const QTime& t,Priority const& p)
    : Job(s,t,p), myMenu(nullptr), maxJobs(0) {
    folderName=name;
    checkFolderName();
}

jobOverlord::jobOverlord(std::string const& s,const QDate& d,const QTime& t,Priority const& p)
    : Job(s,d,t,p), myMenu(nullptr), maxJobs(0) {
    folderName=name;
    checkFolderName();
}

jobOverlord::jobOverlord()
    : myMenu(nullptr), maxJobs(0)
{}

jobOverlord::~jobOverlord() {}

void jobOverlord::complete() const {
    QDir MainFolder(QDir::homePath()+"/.prioriter/savedJobs");

    remove(QDir::toNativeSeparators(MainFolder.absoluteFilePath((folderName+".txt").c_str())).toStdString().c_str());
}

void jobOverlord::add_child(std::string const& s) {
    babies.push_back(new Job(s));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_child(std::string const& s,QDateTime const& d) {
    babies.push_back(new Job(s,d));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_child(std::string const& s,QDateTime const& d,Priority const& p) {
    babies.push_back(new Job(s,d,p));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_child(std::string const& s,const QDate& d) {
    babies.push_back(new Job(s,d));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_child(std::string const& s,const QTime& t) {
    babies.push_back(new Job(s,t));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_child(std::string const& s,const QDate& d,const QTime& t) {
    babies.push_back(new Job(s,d,t));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_child(std::string const& s,const QDate& d,Priority const& p) {
    babies.push_back(new Job(s,d,p));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_child(std::string const& s,const QTime& t,Priority const& p) {
    babies.push_back(new Job(s,t,p));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_child(std::string const& s,const QDate& d,const QTime& t,Priority const& p) {
    babies.push_back(new Job(s,d,t,p));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_child(Job*const& j) {
    babies.push_back(j);
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}


void jobOverlord::add_deadlined_child(std::string const& s) {
    babies.push_back(new deadlinedJob(s));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_deadlined_child(std::string const& s,QDateTime const& d) {
    babies.push_back(new deadlinedJob(s,d));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_deadlined_child(std::string const& s,QDateTime const& d,Priority const& p) {
    babies.push_back(new deadlinedJob(s,d,p));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_deadlined_child(std::string const& s,const QDate& d) {
    babies.push_back(new deadlinedJob(s,d));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_deadlined_child(std::string const& s,const QTime& t) {
    babies.push_back(new deadlinedJob(s,t));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_deadlined_child(std::string const& s,const QDate& d,const QTime& t) {
    babies.push_back(new deadlinedJob(s,d,t));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_deadlined_child(std::string const& s,const QDate& d,Priority const& p) {
    babies.push_back(new deadlinedJob(s,d,p));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_deadlined_child(std::string const& s,const QTime& t,Priority const& p) {
    babies.push_back(new deadlinedJob(s,t,p));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_deadlined_child(std::string const& s,const QDate& d,const QTime& t,Priority const& p) {
    babies.push_back(new deadlinedJob(s,d,t,p));
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

void jobOverlord::add_deadlined_child(deadlinedJob*const& j) {
    babies.push_back(j);
    babies.back()->setParent(this);
    std::sort(babies.begin(),babies.end(),compareJobs);
    recalculateMaxJobs();
}

bool jobOverlord::updatePriority() {
    bool change(false);
    for (size_t i(0); i<babies.size(); ++i) {
        change=change || babies[i]->updatePriority();
    }
    if (change) {
        std::sort(babies.begin(),babies.end(),compareJobs);
    }
    return change;
}

void jobOverlord::writeJob(const QString &s) const {
    std::ofstream writeFile;
    writeFile.open(s.toStdString(),std::ofstream::out | std::ofstream::app);
    writeFile << "o " << aRendre << " " << priorite << " " << folderName << std::endl;
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

QAction* jobOverlord::addToMenu(QMenu* menu) {
    myMenu = new QMenu(name.c_str(),menu);
    /*for (size_t i(0); i<babies.size(); ++i) {
        babies[i]->addToMenu(subMenu);
    }*/
    //menu->addMenu(myMenu);
    return myMenu->menuAction();
}

void jobOverlord::removeJob(Job* job) {
    for(size_t i(0); i<babies.size(); ++i) {
        if (babies[i]==job) {
            babies[i]=babies.back();
            babies.pop_back();
            
            recalculateMaxJobs();
            //createMenu();
            return;
        }
    }
}

void jobOverlord::removeJob(int jAddress) {
    Job* job;
    bool found(false);
    for (size_t i(0); i<deleteHelper.size() && !found; ++i) {
        if (deleteHelper[i].first==jAddress) {
            job=deleteHelper[i].second;
            deleteHelper[i]=deleteHelper.back();
            deleteHelper.pop_back();
            found=true;
        }
    }
    if (!found) return;
    for(size_t i(0); i<babies.size(); ++i) {
        if (babies[i]==job) {
            babies[i]=babies.back();
            babies.pop_back();
            recalculateMaxJobs();
            //createMenu();
            return;
        }
    }
}

Job* jobOverlord::getCopy() const {
    return new jobOverlord(*this);
}

bool jobOverlord::DoLater(Job const& j) const {
    if (babies.size()>0) {
        bool subJobImportanceTooLow(Job::DoLater(babies[0]));
        if (subJobImportanceTooLow) {
            return do_sooner(j.due(),j.getPriority(),j.Name(),false,j.getWeight(),aRendre,priorite,name,false,weight);
        }
        else return babies[0]->DoLater(j);
    }
    else {
        return do_sooner(j.due(),j.getPriority(),j.Name(),false,j.getWeight(),aRendre,priorite,name,false,weight);
    }
}

bool jobOverlord::DoLater(deadlinedJob const& j) const {
    if (babies.size()>0) {
        bool subJobImportanceTooLow(Job::DoLater(babies[0]));
        if (subJobImportanceTooLow) {
            return do_sooner(j.due(),j.getPriority(),j.Name(),true,j.getWeight(),aRendre,priorite,name,false,weight);
        }
        else return babies[0]->DoLater(j);
    }
    else {
        return do_sooner(j.due(),j.getPriority(),j.Name(),true,j.getWeight(),aRendre,priorite,name,false,weight);
    }
}

bool jobOverlord::DoLater(jobOverlord const& j) const {
    if (babies.size()>0) {
        bool subJobImportanceTooLow(Job::DoLater(babies[0]));
        if (subJobImportanceTooLow) return Job::DoLater(j);
        else return babies[0]->DoLater(j);
    }
    else {
        return Job::DoLater(j);
    }
}


bool jobOverlord::DoLater(deadlinedJobOverlord const& j) const {
    if (babies.size()>0) {
        bool subJobImportanceTooLow(Job::DoLater(babies[0]));
        //std::cout << "subjob importance " << subJobImportanceTooLow << std::endl;
        if (subJobImportanceTooLow) return Job::DoLater(j);
        else return babies[0]->DoLater(j);
    }
    else {
        return Job::DoLater(j);
    }
}

bool jobOverlord::DoLater(Job* j) const {
    return j->DoLater(*this);
}

std::vector<jobOverlord*> jobOverlord::getJobTypes() const {
    std::vector<jobOverlord*> jobs;
    for (auto babes: babies) {
        if (babes->isOverlord()) {
            jobOverlord* convertedBabe(dynamic_cast<jobOverlord*>(babes));
            std::vector<jobOverlord*> subJobs(convertedBabe->getJobTypes());
            for (auto sub : subJobs) {
                jobs.push_back(sub);
            }
            jobs.push_back(convertedBabe);
        }
    }
    return jobs;
}

Priority jobOverlord::getMaxPriority() const {
    Priority p(priorite);
    for (auto babes: babies) {
        p=std::max(p,babes->getMaxPriority());
    }
    return p;
}

/*int jobOverlord::remainingJobs() const {
    int complJobs(0);
    for (auto job: babies) {
        complJobs+=job->remainingJobs();
    }
    return complJobs;
}*/

int jobOverlord::jobSize() const {
    return maxJobs;
}

void jobOverlord::recalculateMaxJobs() {
    maxJobs=0;
    for (auto jobs: babies) {
        maxJobs+=jobs->jobSize();
    }
    if (parent!=nullptr) {
        parent->recalculateMaxJobs();
    }
    if (maxJobs==0) weight=1;
    else weight=std::min(1.0/maxJobs,1.0);
}

void jobOverlord::checkFolderName() {
    QDir MainFolder(QDir::homePath()+"/.prioriter/savedJobs");
    std::ifstream folder(QDir::toNativeSeparators(MainFolder.absoluteFilePath((folderName+".txt").c_str())).toStdString(),std::ofstream::in);
    while (folder.good()) {
        folder.close();
        folderName+=std::to_string(rand()%100);
        folder.open(QDir::toNativeSeparators(MainFolder.absoluteFilePath((folderName+".txt").c_str())).toStdString(),std::ofstream::in);
    }
}

std::string jobOverlord::BigName() const {
    std::string result;
    if (parent!=nullptr) {
        result+=parent->BigName();
        result+=" > ";
    }
    result+=name;
    return result;
}

jobOverlord* jobOverlord::makeJob(Job* j) {
    if (j->hasFixedDeadline()) {
        if (j->isOverlord()) {
            return new jobOverlord(dynamic_cast<deadlinedJobOverlord*>(j));
        }
        else {
            return new jobOverlord(dynamic_cast<deadlinedJob*>(j));
        }
    }
    else {
        if (j->isOverlord()) {
            return new jobOverlord(dynamic_cast<jobOverlord*>(j));
        }
        else {
            return new jobOverlord(j);
        }
    }
}
