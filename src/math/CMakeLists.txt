
file(GLOB_RECURSE math_src *.cpp)

add_library(
    GtkMath 
    ${math_src}
)

target_include_directories(
    GtkMath
    PUBLIC ${gtk_inner_include_dir}/math
           linarg
)