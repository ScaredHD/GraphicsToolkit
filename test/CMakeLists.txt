include(GoogleTest)

add_executable(
    math_test
    mat/mat_vec_test.cpp
)

target_link_libraries(
    math_test
    PRIVATE GTest::gtest_main
            GtkMath
)

gtest_discover_tests(math_test)

add_executable(
    core_test
    core/ref_test.cpp
)

target_link_libraries(
    core_test
    PRIVATE GTest::gtest_main
            GtkCore
)

gtest_discover_tests(core_test)