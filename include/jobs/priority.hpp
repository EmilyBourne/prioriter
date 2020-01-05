#ifndef PRIORITY_H
#define PRIORITY_H

enum Priority
{
    LOW,
    MEDIUM,
    HIGH
};

int compare(Priority, Priority);
Priority nextPriority(Priority p);

#endif // PRIORITY_H
