file(GLOB_RECURSE test_src *.cpp)

add_executable(
    gtk_tests
    ${test_src}
)

target_link_libraries(
    gtk_tests
    PRIVATE GTest::gtest_main
            GtkMath
            GtkRandom
            GtkScene
            GtkCore
)


include(GoogleTest)
gtest_discover_tests(gtk_tests)