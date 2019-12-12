#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <string_tools.hpp>

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
