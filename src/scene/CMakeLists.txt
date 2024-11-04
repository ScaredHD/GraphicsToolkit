
file(GLOB scene_src *.cpp)

add_library(
    GtkScene
    ${scene_src}
)

target_link_libraries(
    GtkScene
    PRIVATE tinyobjloader
            fmt
            GtkMath
)

target_include_directories(
    GtkScene
    PUBLIC ${gtk_internal_include_directory}/scene
)