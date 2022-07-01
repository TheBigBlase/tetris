cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    tetris
    VERSION 0.1.0
    DESCRIPTION "tetris but its shit"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES C
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    tetris_lib OBJECT
    source/lib.c
		source/tetris.c
)

target_include_directories(
    tetris_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(tetris_lib PUBLIC c_std_99)

# ---- Declare executable ----

add_executable(tetris_exe source/main.c)
add_executable(tetris::exe ALIAS tetris_exe)

set_target_properties(
    tetris_exe PROPERTIES
    OUTPUT_NAME tetris
    EXPORT_NAME exe
)

target_compile_features(tetris_exe PRIVATE c_std_99)

target_link_libraries(tetris_exe PRIVATE tetris_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT tetris_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of tetris"
  )
endif()

include(cmake/dev-mode.cmake)
