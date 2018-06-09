#include "recurringmanager.h"

recurringManager::recurringManager(int h, int mi, int s, int d, int mo, int y)
    : hours(h), mins(mi), secs(s), days(d), months(mo), years(y)
{
    
}

Job* recurringManager::update() {
    QDateTime today(QDateTime::currentDateTime());
    if (nextJobCreation<today) {
        nextJobCreation.addDays(days);
        nextJobCreation.addMonths(months);
        nextJobCreation.addYears(years);
        nextJobCreation.addSecs(hours*3600);
        nextJobCreation.addSecs(mins*60);
        nextJobCreation.addSecs(secs);
        if (overlord) {
            if (fixedDeadline) {
                
            }
            else {
                
            }
        }
        else {
            if (fixedDeadline) {
                
            }
            else {
                
            }
        }
    }
    else {
        return nullptr;
    }
}
