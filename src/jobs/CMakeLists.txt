
SET( JOB_SRC
    deadlined_jobs.cpp
    job_group.cpp
    jobs.cpp
    priority.cpp
    job_comparisons.cpp
    job_interface.cpp
)

add_library( jobs OBJECT ${JOB_SRC})
target_include_directories( jobs
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/jobs
)
target_link_libraries( jobs tools )
