#ifndef DEADLINED_JOBS_H
#define DEADLINED_JOBS_H

#include <priority.hpp>
#include <jobs.hpp>

#include <time.h>

class DeadlinedJobs: public Jobs
{
    public:
        DeadlinedJobs(std::string const& name, Priority priority, time_t doDate);
        time_t getDeadline() const;
        virtual void update() override;
        virtual int compare_with(std::shared_ptr<const JobInterface> const&, bool = true, bool = true) const override;
        virtual int compare_with(Jobs const&, bool = true, bool = true) const override;
        virtual int compare_with(DeadlinedJobs const&, bool = true, bool = true) const override;
        virtual int compare_with(JobGroup const&, bool = true, bool = true) const override;
    protected:
        void getNextUpdateTime();
        time_t m_deadline;
        time_t next_change;
};

#endif // DEADLINED_JOBS_H
