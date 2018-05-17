#include "addoverlord.h"
#include "deadlinedjoboverlord.h"

AddOverlord::AddOverlord(std::vector<jobOverlord*> const& overlords,jobOverlord* index)
    :job(nullptr), posInOverlord(-1)
{
    setupUi(this);
    priorityEdit->setCurrentIndex(2);
    deadlineEdit->setDate(QDate::currentDate());
    deadlineEdit->setTime(QTime::currentTime());
    deadlineEdit->setCalendarPopup(true);
    for (size_t i(0); i<overlords.size(); ++i) {
        overlordList->addItem(overlords[i]->BigName().c_str());
        if (overlords[i]==index) overlordList->setCurrentIndex(i+1);
    }
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(makeJob()));
}

AddOverlord::~AddOverlord() {

}

void AddOverlord::makeJob() {
    std::string name(nameEdit->text().toStdString());
    Priority p(Priority(priorityEdit->currentIndex()));

    if (hasDeadline->isChecked()) {
        job = new deadlinedJobOverlord(name,deadlineEdit->date(),deadlineEdit->time(),p);
    }
    else {
        job = new jobOverlord(name,deadlineEdit->date(),deadlineEdit->time(),p);
    }
}

bool AddOverlord::addToOverlord(std::vector<jobOverlord*>& heads) {
    std::string Dad(overlordList->currentText().toStdString());
    if (Dad=="") {
        heads.push_back(job);
        return false;
    }
    else {
        for (size_t i(0); i<heads.size(); ++i) {
            if (Dad==heads[i]->BigName()) {
                heads[i]->add_child(job);
                heads.push_back(job);
                posInOverlord=i;
                return true;
            }
        }
        std::cerr << "Couldn't find '" << Dad << "'" << std::endl;
        return false;
    }
}
