#ifndef JOB_COMPARISONS_H
#define JOB_COMPARISONS_H
#include <memory>
#include <jobs.hpp>
#include <deadlined_jobs.hpp>
#include <job_group.hpp>

int compare(std::weak_ptr<const JobInterface> const&, std::weak_ptr<const JobInterface> const&, bool = true, bool = true);

int compare(Jobs const&, std::shared_ptr<const JobInterface> const&, bool = true, bool = true);
int compare(DeadlinedJobs const&, std::shared_ptr<const JobInterface> const&, bool = true, bool = true);
int compare(JobGroup const&, std::shared_ptr<const JobInterface> const&, bool = true, bool = true);

int compare(Jobs const&, Jobs const&, bool = true, bool = true);
int compare(Jobs const&, DeadlinedJobs const&, bool = true, bool = true);
int compare(Jobs const&, JobGroup const&, bool = true, bool = true);

int compare(DeadlinedJobs const&, Jobs const&, bool = true, bool = true);
int compare(DeadlinedJobs const&, DeadlinedJobs const&, bool = true, bool = true);
int compare(DeadlinedJobs const&, JobGroup const&, bool = true, bool = true);

int compare(JobGroup const&, Jobs const&, bool = true, bool = true);
int compare(JobGroup const&, DeadlinedJobs const&, bool = true, bool = true);
int compare(JobGroup const&, JobGroup const&, bool = true, bool = true);

#endif // JOB_COMPARISONS_H
