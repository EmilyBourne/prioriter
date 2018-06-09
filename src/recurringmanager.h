#ifndef RECURRINGMANAGER_H
#define RECURRINGMANAGER_H
#include <QDateTime>
#include "job.h"

class recurringManager
{
public:
    recurringManager(int h, int mi, int s, int d, int mo, int y);
    Job* update();
private:
    bool overlord;
    bool fixedDeadline;
    QDateTime nextJobCreation;
    int hours;
    int mins;
    int secs;
    int days;
    int months;
    int years;
};

#endif // RECURRINGMANAGER_H
