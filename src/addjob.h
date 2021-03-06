#ifndef ADDJOB_H
#define ADDJOB_H
#include "ui_addjob.h"
#include "job.h"
#include <vector>

class AddNewJob: public QDialog, protected Ui::addJob
{
    Q_OBJECT
public:
    AddNewJob(const std::vector<jobOverlord*> &overlords,jobOverlord*);
    ~AddNewJob();
    Job* getJob() const {return job; }
    bool addToOverlord(std::vector<jobOverlord*> const&);
    int getPosInOverlord() {return posInOverlord; }
private slots:
    void makeJob();
    void dateChoose(const QDateTime & );
    void changeOverlord(int index);
private:
    Job* job;
    int posInOverlord;
    bool dateChosen;
    std::vector<jobOverlord*> const& overlords;
};

#endif // ADDJOB_H
