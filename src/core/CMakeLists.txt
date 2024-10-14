
add_library(
    GtkCore
    INTERFACE
)

target_include_directories(
    GtkCore
    INTERFACE ref_ptr
)