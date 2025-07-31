
add_subdirectory(Math)

add_library(GtkCore INTERFACE)

target_include_directories(
    GtkCore
    INTERFACE
        ${gtk_inner_include_dir}/Core
)

add_subdirectory(Gui)