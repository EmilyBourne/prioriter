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
        virtual int compare_with(std::shared_ptr<JobInterface> const&) const override;
        virtual int compare_with(Jobs const&) const override;
        virtual int compare_with(DeadlinedJobs const&) const override;
        virtual int compare_with(JobGroup const&) const override;
    protected:
        double getMultiplicationFactor() {return multiplication_factor; }
        void setMultiplicationFactor(double mf) {multiplication_factor=mf; }
        double multiplication_factor;
        JobInterface* description;
        //list<JobInterface*> children;
};

#endif // JOB_GROUP_H
