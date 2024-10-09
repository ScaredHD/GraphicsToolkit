
add_library(GtkMath utils.cpp)

target_include_directories(
    GtkMath
    PUBLIC ${PROJECT_SOURCE_DIR}/include/GraphicsToolkit/math
    PRIVATE ${PROJECT_SOURCE_DIR}/include/math
    PRIVATE ${PROJECT_SOURCE_DIR}/include/math/impl
)