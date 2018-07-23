cmake_minimum_required(VERSION 3.8)
project(pride)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/scaffold")
include(scaffold)

sf_check_master_project(PRIDE_MASTER_PROJECT)
option(PRIDE_ENABLE_BENCHMARKS "Enable building and testing benchmarks for pride" ${PRIDE_MASTER_PROJECT})
option(PRIDE_ENABLE_SAMPLES "Enable building and testing samples for pride" ${PRIDE_MASTER_PROJECT})
option(PRIDE_ENABLE_TESTING "Enable building and testing test for pride" ${PRIDE_MASTER_PROJECT})

add_subdirectory(pride)

if (PRIDE_ENABLE_BENCHMARKS AND NOT NO_TESTS)
  add_subdirectory(benchmarks)
endif()

if (PRIDE_ENABLE_SAMPLES)
  add_subdirectory(samples)
endif()

if (PRIDE_ENABLE_TESTING AND NOT NO_TESTS)
  add_subdirectory(tests)
endif()
