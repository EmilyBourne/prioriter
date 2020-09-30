
cmake_minimum_required( VERSION 3.12 )
project( prioriter CXX )

add_library( doctest INTERFACE )
target_include_directories( doctest
    INTERFACE
    ${PROJECT_SOURCE_DIR}/include/extern
)

add_library( tools INTERFACE )
target_include_directories( tools
    INTERFACE
    ${PROJECT_SOURCE_DIR}/include/tools
)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
