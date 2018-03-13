include(clone)

git_clone(celero https://github.com/CruizeMissile/celero "${CMAKE_SOURCE_DIR}/external")

option(CELERO_COMPILE_DYNAMIC_LIBRARIES "Set to ON to build Celero for dynamic linking.  Use OFF for static." OFF)
option(CELERO_RUN_EXAMPLE_ON_BUILD "Set to ON to automatically run the example after a successful build." OFF)
option(CELERO_ENABLE_EXPERIMENTS "Set to ON to automatically build all examples." OFF)
option(CELERO_ENABLE_FOLDERS "Enable to put Celero in its own solution folder under Visual Studio" ON)
option(CELERO_ENABLE_TESTS "Enable building and running unit tests." OFF)

add_subdirectory("${CMAKE_SOURCE_DIR}/external/celero" "${CMAKE_BINARY_DIR}/external/celero")

add_executable(bench bench.cpp)
target_link_libraries(bench celero libpride)
