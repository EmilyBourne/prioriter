#ifndef DEADLINED_JOBS_H
#define DEADLINED_JOBS_H

#include <priority.hpp>
#include <jobs.hpp>

class DeadlinedJobs: public Jobs
{
    public:
        DeadlinedJobs(std::string const& name, Priority priority);
        virtual std::string getName() const;
        virtual Priority getPriority() const;
        virtual int compare_with(std::shared_ptr<JobInterface> const&) const override;
        virtual int compare_with(Jobs const&) const override;
        virtual int compare_with(DeadlinedJobs const&) const override;
        virtual int compare_with(JobGroup const&) const override;
};

#endif // DEADLINED_JOBS_H
