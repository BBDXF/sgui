cmake_minimum_required(VERSION 3.10)
project(SGUI VERSION 0.1.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 启用位置无关代码
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 寻找GLFW
find_package(glfw3 REQUIRED)

# 包含目录
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${GLFW3_INCLUDE_DIRS}
)

# 源代码目录
set(SGUI_SOURCES
    # 核心组件
    ${PROJECT_SOURCE_DIR}/src/core/
    # 控件
    ${PROJECT_SOURCE_DIR}/src/controls/
    # 布局
    ${PROJECT_SOURCE_DIR}/src/layout/
    # 窗口管理
    ${PROJECT_SOURCE_DIR}/src/window/
    # 内部实现
    ${PROJECT_SOURCE_DIR}/src/internal/
)

# 示例程序
add_subdirectory(examples)

# 主库目标（暂时注释，因为项目刚开始，没有实际源代码）
# add_library(sgui STATIC
#     # 这里将添加具体的源文件
#     # 目前为空，因为项目刚开始
# )

# 链接依赖（暂时注释）
# target_link_libraries(sgui
#     PRIVATE
#     ${GLFW3_LIBRARIES}
# )

# 安装规则（暂时注释）
# install(TARGETS sgui
#     ARCHIVE DESTINATION lib
#     LIBRARY DESTINATION lib
# )

# install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
#     DESTINATION include
# )
