cmake_minimum_required(VERSION 3.14)

project(
    tetris
    VERSION 0.1.0
    DESCRIPTION "tetris but its shit"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES C
)

set(src
	src/main.c
	src/tetris.c
	)



set(CMAKE_DEBUG_POSTFIX d)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(tetris ${src})

set_target_properties(tetris PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_compile_features(tetris PRIVATE c_std_99)
target_include_directories(tetris PRIVATE ${CURSES_LIBRARIES})
target_link_libraries(tetris ${CURSES_LIBRARIES})
