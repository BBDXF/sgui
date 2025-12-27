# Input Demo CMakeLists.txt

# 创建可执行文件
add_executable(input_demo main.cpp)

# 链接SGUI库
target_link_libraries(input_demo 
    PRIVATE 
    sgui::sgui
)

# 设置包含目录
target_include_directories(input_demo 
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)

# 如果是Debug模式，添加调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(input_demo PRIVATE DEBUG)
endif()
