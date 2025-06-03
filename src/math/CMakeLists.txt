
file(GLOB_RECURSE math_src *.cpp)

add_library(
    GtkMath 
    ${math_src}
)

target_include_directories(
    GtkMath
    PUBLIC ${gtk_internal_include_directory}/math
           linarg
)