#ifndef JOBS_H
#define JOBS_H

#include <priority.hpp>
#include <job_interface.hpp>

class Jobs: public JobInterface
{
    public:
        Jobs(std::string const& name, Priority priority);
        virtual std::string getName() const override;
        virtual std::string& setName() override;
        virtual Priority getPriority() const override;
        virtual int compare_with(std::shared_ptr<const JobInterface> const&, bool = true, bool = true) const override;
        virtual int compare_with(Jobs const&, bool = true, bool = true) const override;
        virtual int compare_with(DeadlinedJobs const&, bool = true, bool = true) const override;
        virtual int compare_with(JobGroup const&, bool = true, bool = true) const override;
    protected:
        double getMultiplicationFactor() {return multiplication_factor; }
        void setMultiplicationFactor(double mf) {multiplication_factor=mf; }

        std::string m_name;
        Priority m_priority;
        double multiplication_factor;
};

#endif // JOBS_H
