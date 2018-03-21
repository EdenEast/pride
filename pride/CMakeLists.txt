
add_library(libpride INTERFACE)
target_include_directories(libpride INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
sf_set_cxxstd(libpride 17)
set_compile_flags(libpride)

# custom target for ides
file(GLOB_RECURSE pride_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
add_custom_target(pride SOURCES "${pride_sources}")
add_filter_group("${pride_sources}" "${CMAKE_CURRENT_SOURCE_DIR}")
