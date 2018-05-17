#include "editjob.h"
#include "job.h"
#include "joboverlord.h"
#include "deadlinedjob.h"
#include "deadlinedjoboverlord.h"
#include "window.h"

EditJob::EditJob(Window* w, std::vector<jobOverlord *> &overlords, jobTreeItem *j)
    : window(w), jobPtr(j->getJob()), manager(j), daddy(nullptr), Overlords(overlords) {
    setupUi(this);
    priorityEdit->setCurrentIndex(jobPtr->getPriority());
    deadlineEdit->setDate(jobPtr->due().date());
    deadlineEdit->setTime(jobPtr->due().time());
    deadlineEdit->setCalendarPopup(true);
    int index(-1);
    int k(1);
    dropdownToOverlordMapper[0]=0;
    if (j->getParent()!=nullptr) {
        daddy=dynamic_cast<jobOverlord*>(j->getParent()->getJob());
        for (size_t i(0); i<overlords.size(); ++i) {
            std::string newName(overlords[i]->BigName().c_str());
            if (newName.compare(jobPtr->BigName())!=0 && newName.find(jobPtr->BigName()+" > ")!=0) {
                overlordList->addItem(newName.c_str());
                if (overlords[i]->Name()==daddy->Name()) {
                    index=k;
                }
                dropdownToOverlordMapper[k++]=i+1;
            }
        }
    }
    else {
        for (size_t i(0); i<overlords.size(); ++i) {
            std::string newName(overlords[i]->BigName().c_str());
            if (newName.compare(jobPtr->BigName())!=0 && newName.find(jobPtr->BigName()+" > ")!=0) {
                overlordList->addItem(newName.c_str());
                dropdownToOverlordMapper[k++]=i+1;
            }
        }
    }
    overlordList->setCurrentIndex(index);
    oldIndex=index;
    nameEdit->setText(jobPtr->Name().c_str());
    hasDeadline->setChecked(jobPtr->hasFixedDeadline());
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(modifyJob()));
}

EditJob::~EditJob() {}

void EditJob::modifyJob() {
    Job* newJob;
    if (jobPtr->isOverlord()) {
        if (hasDeadline->isChecked()) {
            newJob=deadlinedJobOverlord::makeJob(jobPtr);
        }
        else {
            newJob=jobOverlord::makeJob(jobPtr);
        }
        jobOverlord* newJobO(dynamic_cast<jobOverlord*>(newJob));
        for (auto& over: Overlords) {
            if (over==jobPtr) {
                over=newJobO;
            }
        }
        dynamic_cast<jobOverlord*>(jobPtr)->complete();
    }
    else {
        if (hasDeadline->isChecked()) {
            newJob=deadlinedJob::makeJob(jobPtr);
        }
        else {
            newJob=Job::makeJob(jobPtr);
        }
    }
    newJob->setName()=nameEdit->text().toStdString();
    newJob->setPriority(Priority(priorityEdit->currentIndex()));
    newJob->setARendre(deadlineEdit->dateTime());
    if (daddy!=nullptr) {
        daddy->removeJob(jobPtr);
    }
    delete jobPtr;
    int overlordIndex(dropdownToOverlordMapper[overlordList->currentIndex()]);
    if (overlordIndex!=0) {
        Overlords[overlordIndex-1]->add_child(newJob);
    }
    (*manager->getSettableJob())=newJob;
    newJob->saveParent(manager);
    if (overlordIndex!=oldIndex) {
        jobTreeItem* topDog(manager->getParent());
        if (topDog) {
            topDog->removeChild(manager);
        }
        else {
            window->extractJob(manager);
        }
        if (overlordIndex!=0) {
            Overlords[overlordIndex-1]->getParent()->addSomeonesChild(manager);
        }
        else {
            window->addJob(manager);
            manager->setParent(nullptr);
            newJob->setParent(nullptr);
        }
        manager->reconnect();
    }
}
