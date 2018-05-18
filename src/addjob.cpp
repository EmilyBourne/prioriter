#include "addjob.h"
#include "deadlinedjob.h"
#include "joboverlord.h"
#include <string>
#include <QCalendarWidget>

AddNewJob::AddNewJob(std::vector<jobOverlord*> const& overlords,jobOverlord* index)
    : job(nullptr), posInOverlord(-1)
{
    setupUi(this);
    priorityEdit->setCurrentIndex(1);
    deadlineEdit->setDate(QDate::currentDate());
    deadlineEdit->setTime(QTime::currentTime());
    deadlineEdit->setCalendarPopup(true);
    for (size_t i(0); i<overlords.size(); ++i) {
        overlordList->addItem(overlords[i]->BigName().c_str());
        if (overlords[i]==index) {
            overlordList->setCurrentIndex(i+1);
            deadlineEdit->setDate(index->due().date());
            deadlineEdit->setTime(index->due().time());
        }
    }
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(makeJob()));
}

AddNewJob::~AddNewJob() {

}

void AddNewJob::makeJob() {
    std::string name(nameEdit->text().toStdString());
    Priority p(Priority(priorityEdit->currentIndex()));

    if (hasDeadline->isChecked()) {
        job = new deadlinedJob(name,deadlineEdit->date(),deadlineEdit->time(),p);
    }
    else {
        job = new Job(name,deadlineEdit->date(),deadlineEdit->time(),p);
    }
}

bool AddNewJob::addToOverlord(std::vector<jobOverlord*> const& heads) {
    std::string Dad(overlordList->currentText().toStdString());
    if (Dad=="") {
        return false;
    }
    else {
        for (size_t i(0); i<heads.size(); ++i) {
            if (Dad==heads[i]->BigName()) {
                heads[i]->add_child(job);
                posInOverlord=i;
                return true;
            }
        }
        std::cerr << "Couldn't find '" << Dad << "'" << std::endl;
        return false;
    }
}
