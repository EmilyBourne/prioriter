#ifndef JOB_GROUP_H
#define JOB_GROUP_H

#include <priority.hpp>
#include <job_interface.hpp>

class JobGroup: public JobInterface
{
    public:
        JobGroup(std::string const& name, Priority priority);
        virtual std::string getName() const;
        virtual Priority getPriority() const;
        virtual int doBefore(Jobs const&) const;
        virtual int doBefore(DeadlinedJobs const&) const;
        virtual int doBefore(JobGroup const&) const;
    protected:
        double getMultiplicationFactor() {return multiplication_factor; }
        void setMultiplicationFactor(double mf) {multiplication_factor=mf; }
        double multiplication_factor;
        JobInterface* description;
        //list<JobInterface*> children;
        virtual int doAfter(JobInterface*) override;
};

#endif // JOB_GROUP_H
