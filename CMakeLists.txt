cmake_minimum_required(VERSION 3.20)

project(GraphicsToolkit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_TOP_LEVEL_PROJECT TRUE)
else()
    set(IS_TOP_LEVEL_PROJECT FALSE)
endif()

# Only build testing when working as top level project
if(${IS_TOP_LEVEL_PROJECT})
    enable_testing()
    add_subdirectory(ext/googletest)
    add_subdirectory(test)
endif()

add_subdirectory(ext)

# All submodule libraries
add_subdirectory(src)


# Combine all modules to single target 'Gtk'
add_library(Gtk gtk.cpp)

target_link_libraries(
    Gtk
    INTERFACE GtkMath
              GtkRandom
              GtkScene
)

target_include_directories(
    Gtk 
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)


# playgound executable
add_executable(
    gtk_playground
    gtk_playground.cpp
)

target_link_libraries(
    gtk_playground
    PRIVATE Gtk
            fmt
)