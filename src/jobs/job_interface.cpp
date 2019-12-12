#include <job_interface.hpp>
#include <iostream>

JobInterface::~JobInterface()
{}

bool JobInterface::compareJobs(JobInterface* a, JobInterface* b)
{
    /*
     * return values:
     *  1 : do b first
     *  0 : equivalent
     * -1 : do a first
     */
    return b->doAfter(a) < 0;
}

bool CompareJobs::operator() (JobInterface* a, JobInterface* b) const
{
    return JobInterface::compareJobs(a,b);
}
