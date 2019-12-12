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
        virtual int doBefore(Jobs const&) const;
        virtual int doBefore(DeadlinedJobs const&) const;
        virtual int doBefore(JobGroup const&) const;
    protected:
        virtual int doAfter(JobInterface*) override;
};

#endif // DEADLINED_JOBS_H
