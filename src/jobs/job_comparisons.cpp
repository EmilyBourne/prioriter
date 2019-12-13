#include <job_comparisons.hpp>
#include <string_tools.hpp>

#include <iostream>

/*
 * return values:
 * -1 : do this first
 *  0 : equivalent
 *  1 : do j first
 */

int compare(std::weak_ptr<JobInterface> const& a, std::weak_ptr<JobInterface> const& b)
{
    if (a.expired())
    {
        return b.expired() ? 0 : 1;
    }
    else if (b.expired())
    {
        return -1;
    }
    else
    {
        std::shared_ptr<JobInterface> j1;
        std::shared_ptr<JobInterface> j2;
        j1 = a.lock();
        j2 = b.lock();
        return j1->compare_with(j2);
    }
}

int compare(Jobs const& a, std::shared_ptr<JobInterface> const& b)
{
    return -b->compare_with(a);
}
int compare(DeadlinedJobs const& a, std::shared_ptr<JobInterface> const& b)
{
    return -b->compare_with(a);
}
int compare(JobGroup const& a, std::shared_ptr<JobInterface> const& b)
{
    return -b->compare_with(a);
}

int compareInterface(JobInterface const& a, JobInterface const& b)
{
    if (a.isHoldingUpJobs())
    {
        std::cout << "holding up!!!!!!!!!" << std::endl;
    }
    else if (b.isHoldingUpJobs())
    {
        std::cout << "holding up!!!!!!!!!" << std::endl;
    }
    else return 0;
}

int compare(Jobs const& a, Jobs const& b)
{
    //std::cout << "job job" << std::endl;
    int waiting_jobs = compareInterface(a,b);
    if (waiting_jobs != 0) return waiting_jobs;
    if (a.getPriority() != b.getPriority())
    {
        return compare(a.getPriority(), b.getPriority());
    }
    else
    {
        return compare(a.getName(), b.getName());
    }
}
int compare(Jobs const& a, DeadlinedJobs const& b){}
int compare(Jobs const& a, JobGroup const& b){}

int compare(DeadlinedJobs const& a, Jobs const& b)
{
    return -compare(b,a);
}
int compare(DeadlinedJobs const& a, DeadlinedJobs const& b){}
int compare(DeadlinedJobs const& a, JobGroup const& b){}

int compare(JobGroup const& a, Jobs const& b)
{
    return -compare(b,a);
}
int compare(JobGroup const& a, DeadlinedJobs const& b)
{
    return -compare(b,a);
}
int compare(JobGroup const& a, JobGroup const& b){}

/*
int Jobs::doBefore(Jobs const& j) const
{
}

int Jobs::doBefore(DeadlinedJobss const& j) const
{
    return -j.doBefore( const&this);
}

int Jobs::doBefore(JobGroup const& j) const
{
    return -j.doBefore( const&this);
}

int Jobs::doAfter(std::shared_ptr<const JobInterface> a) const
{
    return a->doBefore( const&this);
}
*/
