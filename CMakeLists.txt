cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(common)

project(pride)
handle_build_problems()

set(MASTER_PROJECT OFF)
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    set(MASTER_PROJECT ON)
endif()

option(ENABLE_BENCHMARKS "Enable building and testing benchmarks for pride" ${MASTER_PROJECT})
option(ENABLE_SAMPLES "enable building and testing samples for pride" ${MASTER_PROJECT})
option(ENABLE_TESTING "enable building and testing test for pride" ${MASTER_PROJECT})

add_subdirectory(pride)

if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(ENABLE_SAMPLES)
    add_subdirectory(samples)
endif()

if(ENABLE_TESTING)
    add_subdirectory(tests)
endif()
