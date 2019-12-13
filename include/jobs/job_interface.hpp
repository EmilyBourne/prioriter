#ifndef JOB_INTERFACE_H
#define JOB_INTERFACE_H

#include <string>
#include <memory>
#include <list>
#include <priority.hpp>

class Jobs;
class DeadlinedJobs;
class JobGroup;

class JobInterface {
    public:
        virtual ~JobInterface();
        virtual std::string getName() const = 0;
        virtual std::string& setName() = 0;
        virtual Priority getPriority() const = 0;
        virtual int compare_with(std::shared_ptr<JobInterface> const&) const = 0;
        virtual int compare_with(Jobs const&) const = 0;
        virtual int compare_with(DeadlinedJobs const&) const = 0;
        virtual int compare_with(JobGroup const&) const = 0;
        void addWaitingJob(std::shared_ptr<JobInterface>);
        bool isHoldingUpJobs() const;
        void sort_waiting_jobs();
    protected:
        std::list<std::weak_ptr<JobInterface>> jobsWaitingForMe;
};

struct CompareJobs {
    bool operator() (std::weak_ptr<JobInterface>, std::weak_ptr<JobInterface>) const;
};

void sort_jobs(std::list<std::weak_ptr<JobInterface>>);

#endif // JOB_INTERFACE_H
