cmake_minimum_required(VERSION 3.8)
project(pride)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/scaffold")
include(scaffold)

sf_check_master_project(PRIDE_MASTER_PROJECT)
option(PRIDE_ENABLE_BENCHMARKS "Enable building and testing benchmarks for pride" ${PRIDE_MASTER_PROJECT})
option(PRIDE_ENABLE_SAMPLES "Enable building and testing samples for pride" ${PRIDE_MASTER_PROJECT})
option(PRIDE_ENABLE_TESTING "Enable building and testing test for pride" ${PRIDE_MASTER_PROJECT})

option(ENABLE_ASTYLE "Enable formatting with astyle" OFF)
option(ENABLE_CLANGFORMAT "Enable formatting with clang-format" OFF)
option(ENABLE_CLANGTIDY "Enable static analisys with cppcheck" OFF)
option(ENABLE_CPPCHECK "Enable static analisys with cppcheck" OFF)

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

sf_setup_modules()

if(ENABLE_ASTYLE)
  include(astyle)
  sf_astyle_add_target(${PRIDE_LIB})
  sf_astype_create_command()
endif()

if(ENABLE_CLANGFORMAT)
  include(clangformat)
  sf_clangformat_add_target(${PRIDE_LIB})
  sf_clangformat_create_command()
endif()

if(ENABLE_CLANGTIDY)
  include(clangtidy)
  sf_clangtidy_add_target(${PRIDE_LIB})
  sf_clangtidy_create_command()
endif()

if(ENABLE_CPPCHECK)
  include(cppcheck)
  sf_cppcheck_add_target(${PRIDE_LIB})
  sf_cppcheck_create_command()
endif()
