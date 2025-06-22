
add_executable(gui_test WIN32
    gui_test.cpp
)

target_link_libraries(
    gui_test
    PUBLIC
        imgui 
        d3d11.lib
        dxgi.lib
        dxguid.lib
        d3dcompiler.lib
        dwmapi
)