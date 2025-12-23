# Button Demo CMakeLists.txt

# 添加可执行文件
add_executable(button_demo main.cpp)

# 链接SGUI库
target_link_libraries(button_demo
    PRIVATE
    sgui::sgui
)

# 设置包含目录
target_include_directories(button_demo
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# 设置输出目录
set_target_properties(button_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
