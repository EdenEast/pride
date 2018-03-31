
file(GLOB_RECURSE pride_source_files
  "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_interface_library(libpride IDE_TARGET pride SOURCES ${pride_source_files})
target_common_compiler_flags(libpride)
target_include_directories(libpride INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
