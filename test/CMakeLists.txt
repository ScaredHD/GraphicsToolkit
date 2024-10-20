
add_executable(
    test_app
    mat/mat_vec_test.cpp
    random/random_test.cpp
)

target_link_libraries(
    test_app
    PRIVATE GTest::gtest_main
            GtkMath
            GtkRandom
)


include(GoogleTest)
gtest_discover_tests(test_app)