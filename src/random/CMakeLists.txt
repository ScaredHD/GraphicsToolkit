add_library(
    GtkRandom
    sample.cpp
    discrepancy.cpp
)

target_include_directories(
    GtkRandom
    PUBLIC ${gtk_inner_include_dir}/random
)

target_link_libraries(
    GtkRandom
    PRIVATE GtkMath
            fmt
)