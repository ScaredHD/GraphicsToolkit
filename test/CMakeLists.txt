file(GLOB_RECURSE test_src *.cpp)

add_executable(
    gtk_tests
    ${test_src}
)

target_compile_definitions(
    gtk_tests
    PRIVATE GTK_TEST
)

target_link_libraries(
    gtk_tests
    PRIVATE GTest::gtest_main
            GtkMath
            GtkRandom
)

target_include_directories(
    gtk_tests
    PRIVATE .
)


include(GoogleTest)
gtest_discover_tests(gtk_tests)