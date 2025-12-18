# CMakeLists.txt for Katana Parser Demo
cmake_minimum_required(VERSION 3.10)
project(katana_css_demo)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加 katana-parser 库的路径
set(KATANA_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../katana-parser")
set(KATANA_INCLUDE_DIR "${KATANA_ROOT_DIR}/src")
set(KATANA_LIBRARY_DIR "${KATANA_ROOT_DIR}/.libs")

# 包含头文件目录
include_directories(${KATANA_INCLUDE_DIR})

# 链接库目录
link_directories(${KATANA_LIBRARY_DIR})

# 添加 katana 静态库
add_library(katana_static STATIC IMPORTED)
set_target_properties(katana_static PROPERTIES
    IMPORTED_LOCATION "${KATANA_LIBRARY_DIR}/libkatana.a"
)

# 添加 katana 动态库
add_library(katana_shared SHARED IMPORTED)
set_target_properties(katana_shared PROPERTIES
    IMPORTED_LOCATION "${KATANA_LIBRARY_DIR}/libkatana.so"
)

# 创建可执行文件 - 使用完整路径
add_executable(katana_css_demo ../examples/katana_css_demo.cpp)
add_executable(css_to_kv ../examples/css_to_kv.cpp)

# 链接库
target_link_libraries(katana_css_demo 
    katana_static
    # 如果需要使用动态库，可以使用 katana_shared
    # katana_shared
)

target_link_libraries(css_to_kv 
    katana_static
)

# 设置编译选项
target_compile_options(katana_css_demo PRIVATE 
    -Wall 
    -Wextra
)

target_compile_options(css_to_kv PRIVATE 
    -Wall 
    -Wextra
)

# 设置 C 标准（用于 katana 头文件）
set_property(TARGET katana_css_demo PROPERTY C_STANDARD 99)
set_property(TARGET css_to_kv PROPERTY C_STANDARD 99)

# 输出目录
set_target_properties(katana_css_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_target_properties(css_to_kv PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# 添加自定义目标来构建 katana-parser
add_custom_target(build_katana ALL
    COMMAND ${CMAKE_COMMAND} -E env CFLAGS="-std=c99" ${CMAKE_MAKE_PROGRAM}
    WORKING_DIRECTORY ${KATANA_ROOT_DIR}
    COMMENT "Building katana-parser library"
)

# 确保 katana 库在我们的 demo 之前构建
add_dependencies(katana_css_demo build_katana)
add_dependencies(css_to_kv build_katana)

# 打印一些信息
message(STATUS "Katana Parser Demo Configuration:")
message(STATUS "  Katana Root: ${KATANA_ROOT_DIR}")
message(STATUS "  Katana Include: ${KATANA_INCLUDE_DIR}")
message(STATUS "  Katana Library: ${KATANA_LIBRARY_DIR}")
