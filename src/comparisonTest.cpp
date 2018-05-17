#include "job.h"
#include "joboverlord.h"
#include "deadlinedjob.h"
#include "deadlinedjoboverlord.h"
#include <QDateTime>
#include <iostream>
#include <cassert>
#include <array>
#include <string>
using namespace std;

string pPri(Priority p) {
    switch (p) {
    case HIGH: return "High"; break;
    case MEDIUM: return "Medium";
    case LOW: return "Low";
    default: return "";
    }
}

void check(bool b) {
    if (!b) std::cout << "-------------------------------------------------Problem!" << std::endl;
    assert(b);
}

int main() {
    array<Priority,3> priorities ({LOW,MEDIUM,HIGH});

    for (int i(0);i<3; ++i) {
        for (int j(0);j<3; ++j) {
            for (int k(0);k<3; ++k) {
                for (int l(0);l<3; ++l) {
                    Job job("job",QDateTime(QDate(2018,01,16),QTime(00,01,01)),priorities[i]);
                    deadlinedJob dJob("deadlined job",QDateTime(QDate(2018,01,16),QTime(00,01,01)),priorities[j]);
                    jobOverlord oJob("job overlord",QDateTime(QDate(2018,01,16),QTime(00,01,01)),priorities[k]);
                    deadlinedJobOverlord doJob("deadlined job overlord",QDateTime(QDate(2018,01,16),QTime(00,01,01)),priorities[l]);

                    std::cout << "Priorities are : " << pPri(priorities[i]) << " " << pPri(priorities[j]) << " " << pPri(priorities[k])
                               << " " << pPri(priorities[l]) << std::endl;

                    bool comp1(compareJobs(&job,&dJob));
                    bool comp2(compareJobs(&dJob,&job));
                    std::cout << " j  dj " << comp1 << " " << comp2 << std::endl;
                    check(comp1!=comp2);
                    comp1=compareJobs(&job,&oJob);
                    comp2=compareJobs(&oJob,&job);
                    std::cout << " j  oj " << comp1 << " " << comp2 << std::endl;
                    check(comp1!=comp2);
                    comp1=compareJobs(&job,&doJob);
                    comp2=compareJobs(&doJob,&job);
                    std::cout << " j doj " << comp1 << " " << comp2 << std::endl;
                    check(comp1!=comp2);

                    comp1=compareJobs(&dJob,&oJob);
                    comp2=compareJobs(&oJob,&dJob);
                    std::cout << "dj  oj " << comp1 << " " << comp2 << std::endl;
                    check(comp1!=comp2);
                    comp1=compareJobs(&dJob,&doJob);
                    comp2=compareJobs(&doJob,&dJob);
                    std::cout << "dj doj " << comp1 << " " << comp2 << std::endl;
                    check(comp1!=comp2);

                    comp1=compareJobs(&oJob,&doJob);
                    comp2=compareJobs(&doJob,&oJob);
                    std::cout << "oj doj " << comp1 << " " << comp2 << std::endl;
                    check(comp1!=comp2);
                    std::cout << std::endl;
                }
            }
        }
    }

    deadlinedJob job1("Finish Presentation",QDateTime(QDate(2018,01,18),QTime(8,0,0)),Priority(2));
    deadlinedJob job2("Finish Report modifications",QDateTime(QDate(2018,01,13),QTime(23,55,0)),Priority(2));
    bool comp1(compareJobs(&job1,&job2));
    bool comp2(compareJobs(&job2,&job1));
    std::cout << " j  j " << comp1 << " " << comp2 << std::endl;
    return 0;
}
