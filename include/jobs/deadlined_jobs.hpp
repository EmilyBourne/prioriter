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
        virtual int compare_with(std::shared_ptr<const JobInterface> const&, bool = true, bool = true) const override;
        virtual int compare_with(Jobs const&, bool = true, bool = true) const override;
        virtual int compare_with(DeadlinedJobs const&, bool = true, bool = true) const override;
        virtual int compare_with(JobGroup const&, bool = true, bool = true) const override;
};

#endif // DEADLINED_JOBS_H
