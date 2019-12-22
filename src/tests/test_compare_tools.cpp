#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <compare_tools.hpp>

TEST_CASE( "Find equal" )
{
    CHECK(compare("hello","hello")==0);
    CHECK(compare("world","world")==0);
    CHECK(compare("w","w")==0);
}

TEST_CASE( "Compare same length" )
{
    CHECK(compare("hello","hellp")==-1);
    CHECK(compare("world","worla")==1);
    CHECK(compare("hello","world")==-1);
    CHECK(compare("a","b")==-1);
}

TEST_CASE( "Compare different length" )
{
    CHECK(compare("hello","helloooo")==-1);
    CHECK(compare("worlddddd","world")==1);
    CHECK(compare("hell","world")==-1);
    CHECK(compare("a","bcdl")==-1);
}

TEST_CASE( "Compare numeric" )
{
    CHECK(compare("11111","11112")==-1);
    CHECK(compare("10000","01111")==1);
    CHECK(compare("5000","10000")==-1);
    CHECK(compare("exo1","exo10")==-1);
    CHECK(compare("exo 1","exo10")==-1);
    CHECK(compare("1 exo","10 exo")==-1);
}
