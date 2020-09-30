
add_executable( test_compare_tools test_compare_tools.cpp )
target_link_libraries( test_compare_tools jobs doctest tools )
add_test( NAME test_compare_tools COMMAND test_compare_tools )

add_executable( test_jobs test_jobs.cpp )
target_link_libraries( test_jobs jobs doctest tools )
add_test( NAME test_jobs COMMAND test_jobs )

add_executable( test_deadlined_jobs test_deadlined_jobs.cpp )
target_link_libraries( test_deadlined_jobs jobs doctest tools )
add_test( NAME test_deadlined_jobs COMMAND test_deadlined_jobs )
