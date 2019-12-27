#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <string>
#include <jobs.hpp>
#include <priority.hpp>
#include <job_comparisons.hpp>

constexpr Priority priorities[3] = {Priority::HIGH, Priority::MEDIUM, Priority::LOW};
const std::string pNames[3] = {"HIGH", "MEDIUM", "LOW"};

void compareJobPriorityOnly(Priority p1, Priority p2, bool smaller) {
    Jobs job1 ("bla",p1);
    Jobs job2 ("bla",p2);
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
        Jobs job1 ("bla",priorities[i]);
        Jobs job2 ("raa",priorities[i]);
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
        Jobs job1 ("exo 1",priorities[i]);
        Jobs job2 ("exo 10",priorities[i]);
        job1.setName() = "exo 1";
        CHECK(compare(job1,job2) < 0);
    }
}

TEST_CASE( "Dependence test" )
{
    Jobs job1("a",LOW);
    Jobs job2("b",MEDIUM);
    CHECK( compare(job1, job2) > 0 );
    {
        std::shared_ptr<JobInterface> block1 = std::make_shared<Jobs>("c",HIGH);
        job1.addWaitingJob(block1);
        CHECK( compare(job1, job2) < 0 );
        CHECK( compare(job2, job1) > 0 );
        CHECK( compare(job1, block1) < 0 );
    }
    job1.sort_waiting_jobs();
    CHECK( compare(job1, job2) > 0 );
    {
        std::shared_ptr<JobInterface> block1 = std::make_shared<Jobs>("a",MEDIUM);
        job1.addWaitingJob(block1);
        CHECK( compare(job1, job2) < 0 );
        CHECK( compare(job2, job1) > 0 );
        CHECK( compare(job1, block1) < 0 );
    }
    job1.sort_waiting_jobs();
    CHECK( compare(job1, job2) > 0 );
    {
        std::shared_ptr<JobInterface> block1 = std::make_shared<Jobs>("c",MEDIUM);
        job1.addWaitingJob(block1);
        CHECK( compare(job1, job2) < 0 );
        CHECK( compare(job2, job1) > 0 );
        CHECK( compare(job1, block1) < 0 );
    }
    job1.sort_waiting_jobs();
    {
        std::shared_ptr<JobInterface> block1 = std::make_shared<Jobs>("c",LOW);
        job1.addWaitingJob(block1);
        CHECK( compare(job1, job2) > 0 );
        CHECK( compare(job2, job1) < 0 );
        CHECK( compare(job1, block1) < 0 );
    }
    job1.sort_waiting_jobs();
    {
        std::shared_ptr<JobInterface> block1 = std::make_shared<Jobs>("c",HIGH);
        std::shared_ptr<JobInterface> block2 = std::make_shared<Jobs>("c",LOW);
        job1.addWaitingJob(block1);
        job1.addWaitingJob(block2);
        CHECK( compare(job1, job2) < 0 );
        CHECK( compare(job2, job1) > 0 );
        CHECK( compare(job1, block1) < 0 );
        CHECK( compare(job1, block2) < 0 );
    }
}
