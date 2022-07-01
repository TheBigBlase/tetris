# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(tetrisTests LANGUAGES C)

# ---- Tests ----

add_executable(tetris_test source/tetris_test.c)
target_link_libraries(tetris_test PRIVATE tetris_lib)
target_compile_features(tetris_test PRIVATE c_std_99)

add_test(NAME tetris_test COMMAND tetris_test)

# ---- End-of-file commands ----

add_folders(Test)
