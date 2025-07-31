
add_executable(GuiTest
    ImGuiApp.cpp
)

target_link_libraries(
    GuiTest
    PRIVATE
        imgui 
)