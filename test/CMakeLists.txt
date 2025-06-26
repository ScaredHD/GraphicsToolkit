file(GLOB_RECURSE test_src *.cpp)

add_executable(
    GtkTest
    ${test_src}
)

target_compile_definitions(
    GtkTest
    PRIVATE GTK_TEST
)

target_link_libraries(
    GtkTest
    PRIVATE GTest::gtest_main
            GtkMath
            GtkCore
)



include(GoogleTest)
gtest_discover_tests(GtkTest)