
# 创建可执行文件
add_executable(color_test main.cpp)

# 链接SGUI库
target_link_libraries(color_test sgui::sgui)

# 设置包含目录
target_include_directories(color_test PRIVATE ${CMAKE_SOURCE_DIR}/../../include)
