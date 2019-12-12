#ifndef JOBS_H
#define JOBS_H

#include <priority.hpp>
#include <job_interface.hpp>
#include <set>

class Jobs: public JobInterface
{
    public:
        Jobs(std::string const& name, Priority priority);
        virtual std::string getName() const override;
        virtual std::string& setName() override;
        virtual Priority getPriority() const override;
        virtual int doBefore(Jobs const&) const override;
        virtual int doBefore(DeadlinedJobs const&) const override;
        virtual int doBefore(JobGroup const&) const override;
    protected:
        Priority m_priority;
        std::string m_name;
        double getMultiplicationFactor() {return multiplication_factor; }
        void setMultiplicationFactor(double mf) {multiplication_factor=mf; }
        double multiplication_factor;
        std::set<JobInterface*, CompareJobs> jobsWaitingForMe;
        virtual int doAfter(JobInterface*) override;
};

#endif // JOBS_H
