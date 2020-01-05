#include <deadlined_jobs.hpp>
#include <job_comparisons.hpp>

DeadlinedJobs::DeadlinedJobs(std::string const& name, Priority priority, time_t doDate)
    : Jobs(name, priority), m_deadline(doDate)
{
    getNextUpdateTime();
}

time_t DeadlinedJobs::getDeadline() const
{
    return m_deadline;
}

void DeadlinedJobs::update()
{
    time_t now = time(0);
    double time_to_change = difftime(now, next_change);
    if (time_to_change > 0)
    {
        m_priority = nextPriority(m_priority);
        getNextUpdateTime();
    }
}

void DeadlinedJobs::getNextUpdateTime()
{
    time_t now = time(0);
    double factor;
    switch (m_priority)
    {
        case LOW: factor = 1.0/3.0;
                  break;
        case MEDIUM: factor = 0.5;
                     break;
        case HIGH: factor = 2.0;
    }
    double time_to_change = difftime(now, m_deadline) * factor;
    next_change = now + time_to_change;
}

int DeadlinedJobs::compare_with(std::shared_ptr<const JobInterface> const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

int DeadlinedJobs::compare_with(Jobs const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

int DeadlinedJobs::compare_with(DeadlinedJobs const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

int DeadlinedJobs::compare_with(JobGroup const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}
