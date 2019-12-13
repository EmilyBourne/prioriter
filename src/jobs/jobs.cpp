#include <jobs.hpp>
#include <job_comparisons.hpp>

Jobs::Jobs(std::string const& name, Priority priority)
    : m_name(name), m_priority(priority)
{}

std::string Jobs::getName() const
{
    return m_name;
}

std::string& Jobs::setName()
{
    return m_name;
}

Priority Jobs::getPriority() const
{
    return m_priority;
}

int Jobs::compare_with(std::shared_ptr<const JobInterface> const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

int Jobs::compare_with(Jobs const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

int Jobs::compare_with(DeadlinedJobs const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

int Jobs::compare_with(JobGroup const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

