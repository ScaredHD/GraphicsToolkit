
add_subdirectory(math)
add_subdirectory(random)

add_library(GtkCore INTERFACE)

target_include_directories(
    GtkCore
    INTERFACE
        ${gtk_inner_include_dir}/core
)