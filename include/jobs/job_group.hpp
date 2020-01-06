#ifndef JOB_GROUP_H
#define JOB_GROUP_H

#include <priority.hpp>
#include <job_interface.hpp>

class JobGroup: public JobInterface
{
    public:
        JobGroup(std::string const& name, Priority priority);
        virtual std::string getName() const override;
        virtual Priority getPriority() const override;
        virtual void update() override;
        virtual int compare_with(std::shared_ptr<const JobInterface> const&, bool = true, bool = true) const override;
        virtual int compare_with(Jobs const&, bool = true, bool = true) const override;
        virtual int compare_with(DeadlinedJobs const&, bool = true, bool = true) const override;
        virtual int compare_with(JobGroup const&, bool = true, bool = true) const override;
        virtual void sort_waiting_jobs() override;
        virtual bool holdingUp(JobInterface const&) const override;
    protected:
        std::unique_ptr<JobInterface> description;
        std::list<std::weak_ptr<JobInterface>> children;
};

#endif // JOB_GROUP_H
