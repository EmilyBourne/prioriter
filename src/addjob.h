#ifndef ADDJOB_H
#define ADDJOB_H
#include "ui_addjob.h"
#include "job.h"
#include <vector>

class AddJob: public QDialog, protected Ui::addJob
{
    Q_OBJECT
public:
    AddJob(const std::vector<jobOverlord*> &overlords,jobOverlord*);
    ~AddJob();
    Job* getJob() const {return job; }
    bool addToOverlord(std::vector<jobOverlord*> const&);
    int getPosInOverlord() {return posInOverlord; }
private slots:
    void makeJob();
private:
    Job* job;
    int posInOverlord;
};

#endif // ADDJOB_H
