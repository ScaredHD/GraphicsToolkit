
add_executable(ImGuiAppDX11
    ImGuiAppDX11.cpp
)

target_link_libraries(
    ImGuiAppDX11
    PRIVATE
        imgui 
)


add_executable(ImGuiAppDX12
    ImGuiAppDX12.cpp
)

target_link_libraries(
    ImGuiAppDX12
    PRIVATE
        imgui 
)