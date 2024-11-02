
file(GLOB scene_src *.cpp)

add_library(
    GtkScene
    ${scene_src}
)

target_link_libraries(
    GtkScene
    PUBLIC tinyobjloader
    PRIVATE fmt
            GtkMath
)

target_include_directories(
    GtkScene
    PRIVATE ${tinyobjloader_include_dir}
    PUBLIC ${gtk_internal_include_directory}/scene
)