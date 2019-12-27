#include <job_comparisons.hpp>
#include <compare_tools.hpp>

/*
 * return values:
 * -1 : do this first
 *  0 : equivalent
 *  1 : do j first
 */

int compare(std::weak_ptr<const JobInterface> const& a, std::weak_ptr<const JobInterface> const& b, bool useADeps, bool useBDeps)
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
        std::shared_ptr<const JobInterface> j1;
        std::shared_ptr<const JobInterface> j2;
        j1 = a.lock();
        j2 = b.lock();
        return j1->compare_with(j2, useADeps, useBDeps);
    }
}

int compare(Jobs const& a, std::shared_ptr<const JobInterface> const& b, bool useADeps, bool useBDeps)
{
    return -b->compare_with(a, useBDeps, useADeps);
}
int compare(DeadlinedJobs const& a, std::shared_ptr<const JobInterface> const& b, bool useADeps, bool useBDeps)
{
    return -b->compare_with(a, useBDeps, useADeps);
}
int compare(JobGroup const& a, std::shared_ptr<const JobInterface> const& b, bool useADeps, bool useBDeps)
{
    return -b->compare_with(a, useBDeps, useADeps);
}

int compareInterface(JobInterface const& a, JobInterface const& b, bool useADeps, bool useBDeps)
{
    if (useADeps and a.holdingUp(b)) return -1;
    else if (useBDeps and b.holdingUp(a)) return 1;
    if (useADeps && a.isHoldingUpJobs())
    {
        int a_or_child = a.compare_with(a.first_child().lock(), false, useBDeps);
        if (a_or_child > 0)
        {
            return -b.compare_with(a.first_child().lock(), useBDeps, true);
        }
        else
        {
            return 0;
        }
    }
    else if (useBDeps && b.isHoldingUpJobs())
    {
        int b_or_child = b.compare_with(b.first_child().lock(), false, useADeps);
        if (b_or_child >= 0)
        {
            return a.compare_with(b.first_child().lock(), useADeps, true);
        }
        else
        {
            return 0;
        }
    }
    else return 0;
}

int compare(Jobs const& a, Jobs const& b, bool useADeps, bool useBDeps)
{
    int waiting_jobs = compareInterface(a,b,useADeps,useBDeps);
    if (waiting_jobs == 0)
    {
        if (a.getPriority() != b.getPriority())
        {
            return compare(a.getPriority(), b.getPriority());
        }
        else if (a.getMultiplicationFactor() != b.getMultiplicationFactor())
        {
            return -compare(a.getMultiplicationFactor(),b.getMultiplicationFactor());
        }
        else
        {
            return 2 * compare(a.getName(), b.getName());
        }
    }
    else if (abs(waiting_jobs) == 1)
    {
        return waiting_jobs;
    }
    else
    {
        if (a.getMultiplicationFactor() != b.getMultiplicationFactor())
        {
            return -compare(a.getMultiplicationFactor(),b.getMultiplicationFactor());
        }
        else if (a.getPriority() != b.getPriority())
        {
            return compare(a.getPriority(), b.getPriority());
        }
        else
        {
            return 2 * compare(a.getName(), b.getName());
        }
    }
}
int compare(Jobs const& a, DeadlinedJobs const& b, bool useADeps, bool useBDeps){}
int compare(Jobs const& a, JobGroup const& b, bool useADeps, bool useBDeps){}

int compare(DeadlinedJobs const& a, Jobs const& b, bool useADeps, bool useBDeps)
{
    return -compare(b,a,useBDeps,useADeps);
}
int compare(DeadlinedJobs const& a, DeadlinedJobs const& b, bool useADeps, bool useBDeps){}
int compare(DeadlinedJobs const& a, JobGroup const& b, bool useADeps, bool useBDeps){}

int compare(JobGroup const& a, Jobs const& b, bool useADeps, bool useBDeps)
{
    return -compare(b,a,useBDeps,useADeps);
}
int compare(JobGroup const& a, DeadlinedJobs const& b, bool useADeps, bool useBDeps)
{
    return -compare(b,a,useBDeps,useADeps);
}
int compare(JobGroup const& a, JobGroup const& b, bool useADeps, bool useBDeps){}

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

int Jobs::doAfter(std::shared_ptr<const const JobInterface> a) const
{
    return a->doBefore( const&this);
}
*/
