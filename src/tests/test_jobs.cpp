#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <string>
#include <jobs.hpp>
#include <priority.hpp>

constexpr Priority priorities[3] = {Priority::HIGH, Priority::MEDIUM, Priority::LOW};
const std::string pNames[3] = {"HIGH", "MEDIUM", "LOW"};

void compareJobPriorityOnly(Priority p1, Priority p2,bool expected) {
    Jobs job1 ("bla",p1);
    Jobs job2 ("bla",p2);
    CHECK(JobInterface::compareJobs(&job1,&job2)==expected);
    CHECK(JobInterface::compareJobs(&job2,&job1)==!expected);
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
        CHECK(JobInterface::compareJobs(&job1,&job2));
        job1.setName() = "blaa";
        job2.setName() = "bla";
        CHECK(JobInterface::compareJobs(&job2,&job1));
        job1.setName() = "exo 1";
        job2.setName() = "exo 2";
        CHECK(JobInterface::compareJobs(&job1,&job2));
    }
}

TEST_CASE( "Numeric name test" )
{
    for (int i(0); i<3; ++i) {
        Jobs job1 ("exo 1",priorities[i]);
        Jobs job2 ("exo 10",priorities[i]);
        job1.setName() = "exo 1";
        CHECK(JobInterface::compareJobs(&job1,&job2));
    }
}
