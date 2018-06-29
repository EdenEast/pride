
# Clone google benchmark if does not exists
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

add_external_git_repo(
  URL "https://github.com/google/benchmark.git"
  TAG master
  PREFIX "external/benchmark"
  COMMIT 847c0069021ade355b7678a305f3ac4e4d6f7e79
)

target_set_folder(benchmark "Benchmarks/Google")
target_set_folder(benchmark_main "Benchmarks/Google")

add_executable(hash ${CMAKE_CURRENT_LIST_DIR}/hash.cpp)
target_link_libraries(hash ${PRIDE_LIB} benchmark)
target_source_group(hash)
target_set_folder(hash "Benchmarks")
