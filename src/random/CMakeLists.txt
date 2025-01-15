add_library(
    GtkRandom
    sample.cpp
    discrepancy.cpp
)

target_include_directories(
    GtkRandom
    PUBLIC ${gtk_internal_include_directory}/random
)

target_link_libraries(
    GtkRandom
    PRIVATE GtkMath
            fmt
)