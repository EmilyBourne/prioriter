#ifndef ADDOVERLORD_H
#define ADDOVERLORD_H
#include "ui_addjob.h"
#include "joboverlord.h"

class AddOverlord: public QDialog, protected Ui::addJob
{
    Q_OBJECT
public:
    AddOverlord(const std::vector<jobOverlord *> &overlords, jobOverlord *index);
    ~AddOverlord();
    jobOverlord* getJob() const {return job; }
    bool addToOverlord(std::vector<jobOverlord*>& heads);
    int getPosInOverlord() {return posInOverlord; }
private slots:
    void makeJob();
private:
    jobOverlord* job;
    int posInOverlord;
};

#endif // ADDOVERLORD_H
