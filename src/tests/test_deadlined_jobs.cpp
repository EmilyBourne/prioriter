#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <string>
#include <jobs.hpp>
#include <priority.hpp>
#include <job_comparisons.hpp>

constexpr Priority priorities[3] = {Priority::HIGH, Priority::MEDIUM, Priority::LOW};
const std::string pNames[3] = {"HIGH", "MEDIUM", "LOW"};

void compareJobPriorityOnly(Priority p1, Priority p2, bool smaller) {
    DeadlinedJobs job1 ("bla",p1, time(0)+10);
    DeadlinedJobs job2 ("bla",p2, time(0)+10);
    if (smaller)
    {
        CHECK( compare(job1,job2) < 0 );
        CHECK( compare(job2,job1) > 0 );
    }
    else
    {
        CHECK( compare(job1,job2) > 0 );
        CHECK( compare(job2,job1) < 0 );
    }
}

TEST_CASE("Priority test")
{
    for (int i(0); i<3; ++i) {
        for (int j(0); j<i; ++j) {
            compareJobPriorityOnly(priorities[i],priorities[j],false);
        }
        for (int j(i+1); j<3; ++j) {
            compareJobPriorityOnly(priorities[i],priorities[j],true);
        }
    }
}

TEST_CASE( "Alphabetic test" )
{
    for (int i(0); i<3; ++i) {
        DeadlinedJobs job1 ("bla",priorities[i], time(0)+10);
        DeadlinedJobs job2 ("raa",priorities[i], time(0)+10);
        CHECK(compare(job1,job2) < 0);
        job1.setName() = "blaa";
        job2.setName() = "bla";
        CHECK(compare(job2,job1) < 0);
        job1.setName() = "exo 1";
        job2.setName() = "exo 2";
        CHECK(compare(job1,job2) < 0);
        CHECK(compare(job1,job1) == 0);
    }
}

TEST_CASE( "Numeric name test" )
{
    for (int i(0); i<3; ++i) {
        DeadlinedJobs job1 ("exo 1",priorities[i], time(0)+10);
        DeadlinedJobs job2 ("exo 10",priorities[i], time(0)+10);
        job1.setName() = "exo 1";
        CHECK(compare(job1,job2) < 0);
    }
}

TEST_CASE( "Dependence test" )
{
    DeadlinedJobs job1("a",LOW, time(0)+10);
    DeadlinedJobs job2("b",MEDIUM, time(0)+10);
    CHECK( compare(job1, job2) > 0 );
    {
        std::shared_ptr<JobInterface> block1 = std::make_shared<DeadlinedJobs>("c",HIGH, time(0)+10);
        job1.addWaitingJob(block1);
        CHECK( compare(job1, job2) < 0 );
        CHECK( compare(job2, job1) > 0 );
        CHECK( compare(job1, block1) < 0 );
    }
    job1.sort_waiting_jobs();
    CHECK( compare(job1, job2) > 0 );
    {
        std::shared_ptr<JobInterface> block1 = std::make_shared<DeadlinedJobs>("a",MEDIUM, time(0)+10);
        job1.addWaitingJob(block1);
        CHECK( compare(job1, job2) < 0 );
        CHECK( compare(job2, job1) > 0 );
        CHECK( compare(job1, block1) < 0 );
    }
    job1.sort_waiting_jobs();
    CHECK( compare(job1, job2) > 0 );
    {
        std::shared_ptr<JobInterface> block1 = std::make_shared<DeadlinedJobs>("c",MEDIUM, time(0)+10);
        job1.addWaitingJob(block1);
        CHECK( compare(job1, job2) < 0 );
        CHECK( compare(job2, job1) > 0 );
        CHECK( compare(job1, block1) < 0 );
    }
    job1.sort_waiting_jobs();
    {
        std::shared_ptr<JobInterface> block1 = std::make_shared<DeadlinedJobs>("c",LOW, time(0)+10);
        job1.addWaitingJob(block1);
        CHECK( compare(job1, job2) > 0 );
        CHECK( compare(job2, job1) < 0 );
        CHECK( compare(job1, block1) < 0 );
    }
    job1.sort_waiting_jobs();
    {
        std::shared_ptr<JobInterface> block1 = std::make_shared<DeadlinedJobs>("c",HIGH, time(0)+10);
        std::shared_ptr<JobInterface> block2 = std::make_shared<DeadlinedJobs>("c",LOW, time(0)+10);
        job1.addWaitingJob(block1);
        job1.addWaitingJob(block2);
        CHECK( compare(job1, job2) < 0 );
        CHECK( compare(job2, job1) > 0 );
        CHECK( compare(job1, block1) < 0 );
        CHECK( compare(job1, block2) < 0 );
    }
}

TEST_CASE( "Time test" )
{
    DeadlinedJobs job1 ("exo",LOW, time(0)+100);
    DeadlinedJobs job2 ("exo",LOW, time(0)+200);
    CHECK(compare(job1,job2) < 0);
}
