
# Clone google benchmark if does not exists
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

sf_add_external_git_repo(
  URL "https://github.com/google/benchmark.git"
  TAG master
  PREFIX "external/benchmark"
  COMMIT 847c0069021ade355b7678a305f3ac4e4d6f7e79
  OPTIONAL
)

if(NOT sf_git_error_code)
  sf_target_set_folder(benchmark "Benchmarks/Google")
  sf_target_set_folder(benchmark_main "Benchmarks/Google")

  sf_create_executables_per_files(EXTENTIONS "*.cpp" FOLDER "Benchmarks" DEPENDS benchmark ${PRIDE_LIB})
endif()
