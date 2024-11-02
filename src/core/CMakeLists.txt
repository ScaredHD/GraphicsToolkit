
add_library(
    GtkCore
    INTERFACE
)

target_include_directories(
    GtkCore
    INTERFACE ${gtk_internal_include_directory}/core
)