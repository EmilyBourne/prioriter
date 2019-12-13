#include <job_interface.hpp>
#include <job_comparisons.hpp>

JobInterface::~JobInterface()
{}

bool CompareJobs::operator() (std::weak_ptr<JobInterface> a, std::weak_ptr<JobInterface> b) const
{
    if (a.expired() and b.expired()) return 0;
    else if (a.expired()) return 1;
    else if (b.expired()) return -1;
    else
    {
       return compare(a.lock(),b.lock());
    }
}

void JobInterface::sort_waiting_jobs()
{
    sort_jobs(jobsWaitingForMe);
}

void sort_jobs(std::list<std::weak_ptr<JobInterface>> my_list)
{
    for (auto it = my_list.begin(); it != my_list.end(); )
    {
        if (it->expired())
        {
            it = my_list.erase(it);
        }
        else {
            std::shared_ptr<JobInterface> shared(it->lock());
            if (shared->isHoldingUpJobs())
            {
                shared->sort_waiting_jobs();
            }
            it++;
        }
    }
    my_list.sort(CompareJobs());
}

void JobInterface::addWaitingJob(std::weak_ptr<JobInterface> j)
{
    auto it = jobsWaitingForMe.begin();
    bool toInsert (true);
    while (it != jobsWaitingForMe.end())
    {
        if (it->expired())
        {
            it = jobsWaitingForMe.erase(it);
        }
        else {
            if (toInsert && !compare(j, it->lock()))
            {
                jobsWaitingForMe.insert(it,j);
                toInsert = false;
            }
            it++;
        }
    }
    if (toInsert)
    {
        jobsWaitingForMe.insert(it,j);
    }
}

bool JobInterface::isHoldingUpJobs() const
{
    return !jobsWaitingForMe.empty();
}
