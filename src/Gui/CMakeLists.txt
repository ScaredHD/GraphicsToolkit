
add_executable(GuiTest WIN32
    GuiTest.cpp
)

target_link_libraries(
    GuiTest
    PUBLIC
        imgui 
        d3d11.lib
        dxgi.lib
        dxguid.lib
        d3dcompiler.lib
        dwmapi
)