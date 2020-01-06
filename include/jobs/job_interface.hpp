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
        JobInterface(int mf);
        JobInterface(JobInterface const&) = delete;
        virtual std::string getName() const = 0;
        virtual std::string& setName() = 0;
        virtual Priority getPriority() const = 0;
        virtual void update();
        virtual int compare_with(std::shared_ptr<const JobInterface> const&, bool = true, bool = true) const = 0;
        virtual int compare_with(Jobs const&, bool = true, bool = true) const = 0;
        virtual int compare_with(DeadlinedJobs const&, bool = true, bool = true) const = 0;
        virtual int compare_with(JobGroup const&, bool = true, bool = true) const = 0;
        void addWaitingJob(std::weak_ptr<JobInterface>);
        bool isHoldingUpJobs() const;
        virtual void sort_waiting_jobs();
        std::weak_ptr<JobInterface> first_child() const;
        virtual bool holdingUp(JobInterface const&) const;
        int getMultiplicationFactor() const;
    protected:
        void setMultiplicationFactor(int mf);
        std::list<std::weak_ptr<JobInterface>> jobsWaitingForMe;
        int multiplication_factor;
};

struct CompareJobs {
    bool operator() (std::weak_ptr<JobInterface>, std::weak_ptr<JobInterface>) const;
};

void sort_jobs(std::list<std::weak_ptr<JobInterface>>&);

#endif // JOB_INTERFACE_H
