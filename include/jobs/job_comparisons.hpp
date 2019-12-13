#ifndef JOB_COMPARISONS_H
#define JOB_COMPARISONS_H
#include <memory>
#include <jobs.hpp>
#include <deadlined_jobs.hpp>
#include <job_group.hpp>

int compare(std::weak_ptr<JobInterface> const&, std::weak_ptr<JobInterface> const&);

int compare(Jobs const&, std::shared_ptr<JobInterface> const&);
int compare(DeadlinedJobs const&, std::shared_ptr<JobInterface> const&);
int compare(JobGroup const&, std::shared_ptr<JobInterface> const&);

int compare(Jobs const&, Jobs const&);
int compare(Jobs const&, DeadlinedJobs const&);
int compare(Jobs const&, JobGroup const&);

int compare(DeadlinedJobs const&, Jobs const&);
int compare(DeadlinedJobs const&, DeadlinedJobs const&);
int compare(DeadlinedJobs const&, JobGroup const&);

int compare(JobGroup const&, Jobs const&);
int compare(JobGroup const&, DeadlinedJobs const&);
int compare(JobGroup const&, JobGroup const&);

#endif // JOB_COMPARISONS_H
