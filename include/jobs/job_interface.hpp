#ifndef JOB_INTERFACE_H
#define JOB_INTERFACE_H

#include <string>
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
        virtual int doBefore(Jobs const&) const = 0;
        virtual int doBefore(DeadlinedJobs const&) const = 0;
        virtual int doBefore(JobGroup const&) const = 0;
        static bool compareJobs(JobInterface*, JobInterface*);
    protected:
        virtual int doAfter(JobInterface*) = 0;
};

struct CompareJobs {
    bool operator() (JobInterface*, JobInterface*) const;
};

#endif // JOB_INTERFACE_H
