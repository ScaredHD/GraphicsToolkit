file(GLOB_RECURSE test_src *.cpp)

add_executable(
    test_app
    ${test_src}
)

target_link_libraries(
    test_app
    PRIVATE GTest::gtest_main
            GtkMath
            GtkRandom
            GtkScene
)


include(GoogleTest)
gtest_discover_tests(test_app)