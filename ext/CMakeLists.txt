
add_subdirectory(fmt)
    
add_library(imgui
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_win32.cpp
    imgui/backends/imgui_impl_dx11.cpp
)

target_include_directories(imgui 
    PUBLIC imgui
)

target_link_libraries(imgui
    PRIVATE
        d3d11.lib
        dxgi.lib
        dxguid.lib
        d3dcompiler.lib
        dwmapi
)