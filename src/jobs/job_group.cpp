#include <job_group.hpp>
#include <job_comparisons.hpp>
#include <job_comparisons.hpp>

std::string JobGroup::getName() const
{
    return description->getName();
}

Priority JobGroup::getPriority() const
{
    if (children.empty()) return description->getPriority();
    else
    {
        Priority topPriority = children.begin()->lock()->getPriority();
        if (compare(topPriority, description->getPriority())<0)
        {
            return topPriority;
        }
        else
        {
            return description->getPriority();
        }
    }
}

void JobGroup::update()
{
    description->update();
    for (auto child = children.begin(); child != children.end(); )
    {
        if (child->expired())
        {
            child = children.erase(child);
        }
        else
        {
            child->lock()->update();
            child++;
        }
    }
}
void JobGroup::sort_waiting_jobs()
{
    description->sort_waiting_jobs();
    for (auto child : children)
    {
        if (!child.expired())
        {
            child.lock()->sort_waiting_jobs();
        }
    }
}

bool JobGroup::holdingUp(JobInterface const& j) const
{
    for (auto child : children)
    {
        if (!child.expired())
        {
            if (child.lock()->holdingUp(j)) return true;
        }
    }
    return false;
}

int JobGroup::compare_with(std::shared_ptr<const JobInterface> const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

int JobGroup::compare_with(Jobs const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

int JobGroup::compare_with(DeadlinedJobs const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

int JobGroup::compare_with(JobGroup const& j, bool useDeps, bool useJDeps) const
{
    return compare(*this,j, useDeps, useJDeps);
}

