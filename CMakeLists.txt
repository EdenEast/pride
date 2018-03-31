cmake_minimum_required(VERSION 3.1)

project(pride)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/scaffold")
include(scaffold)

check_master_project(PRIDE_MASTER_PROJECT)
option(PRIDE_ENABLE_BENCHMARKS "Enable building and testing benchmarks for pride" ${PRIDE_MASTER_PROJECT})
option(PRIDE_ENABLE_SAMPLES "enable building and testing samples for pride" ${PRIDE_MASTER_PROJECT})
option(PRIDE_ENABLE_TESTING "enable building and testing test for pride" ${PRIDE_MASTER_PROJECT})


add_subdirectory(pride)

if (PRIDE_ENABLE_SAMPLES)
  add_subdirectory(samples)
endif()

if (PRIDE_ENABLE_TESTING)
  add_subdirectory(tests)
endif()
