#include <job_interface.hpp>
#include <job_comparisons.hpp>

JobInterface::JobInterface(int mf)
    : multiplication_factor(mf)
{}

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
    int nJobs = jobsWaitingForMe.size();
    sort_jobs(jobsWaitingForMe);
    int nRemainingJobs = jobsWaitingForMe.size();
    multiplication_factor += (nRemainingJobs - nJobs);
}

void sort_jobs(std::list<std::weak_ptr<JobInterface>>& my_list)
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
            multiplication_factor--;
        }
        else {
            if (toInsert && !compare(j, it->lock()))
            {
                jobsWaitingForMe.insert(it,j);
                multiplication_factor++;
                toInsert = false;
            }
            it++;
        }
    }
    if (toInsert)
    {
        jobsWaitingForMe.insert(it,j);
        multiplication_factor++;
    }
}

bool JobInterface::isHoldingUpJobs() const
{
    return !jobsWaitingForMe.empty();
}

std::weak_ptr<JobInterface> JobInterface::first_child() const
{
    return *jobsWaitingForMe.begin();
}

bool JobInterface::holdingUp(JobInterface const& j) const
{
    for (auto it = jobsWaitingForMe.begin(); it != jobsWaitingForMe.end(); it++)
    {
        if (!it->expired()) {
            if (&j == it->lock().get() or it->lock()->holdingUp(j))
            {
                return true;
            }
        }
    }
    return false;
}

int JobInterface::getMultiplicationFactor() const {
    return multiplication_factor;
}
void JobInterface::setMultiplicationFactor(int mf) {
    multiplication_factor=mf;
}
