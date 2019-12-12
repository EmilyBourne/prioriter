#include <jobs.hpp>
#include <deadlined_jobs.hpp>
#include <job_group.hpp>
#include <string_tools.hpp>

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

int Jobs::doBefore(Jobs const& j) const
{
    /*
     * return values:
     * -1 : do this first
     *  0 : equivalent
     *  1 : do j first
     */
    if (getPriority() != j.getPriority())
    {
        return compare(getPriority(), j.getPriority());
    }
    else
    {
        return compare(getName(), j.getName());
    }
}

int Jobs::doBefore(DeadlinedJobs const& j) const
{
    return -j.doBefore(*this);
}

int Jobs::doBefore(JobGroup const& j) const
{
    return -j.doBefore(*this);
}

int Jobs::doAfter(JobInterface* a)
{
    /*
     * return values:
     *  1 : do this first
     *  0 : equivalent
     * -1 : do a first
     */
    return a->doBefore(*this);
}
