
add_library(GtkMath gtk_utils.cpp)

target_include_directories(
    GtkMath
    PUBLIC ${gtk_internal_include_directory}/math
)