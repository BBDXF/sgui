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

# 添加yoga作为外部依赖
option(BUILD_YOGA "Build yoga library from source" ON)
if(BUILD_YOGA)
    # 构建yoga库
    add_subdirectory(ref/yoga yoga_build)
    
    # 创建yoga的别名，方便其他target使用
    add_library(yoga::yoga ALIAS yogacore)
else()
    # 查找已安装的yoga库
    find_package(yoga REQUIRED)
endif()

# 包含src子目录来构建sgui库
add_subdirectory(src)

# 示例程序
add_subdirectory(examples)
