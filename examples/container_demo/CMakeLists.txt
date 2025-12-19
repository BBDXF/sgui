# SContainer Demo CMakeLists.txt

# 添加container demo可执行文件
add_executable(container_demo
    main.cpp
)

# 包含目录
target_include_directories(container_demo PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/ref/yoga
)

# 设置C++标准
set_property(TARGET container_demo PROPERTY CXX_STANDARD 20)
set_property(TARGET container_demo PROPERTY CXX_STANDARD_REQUIRED ON)
