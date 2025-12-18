# Yoga Demo CMakeLists.txt

# 添加yoga demo可执行文件
add_executable(yoga_demo
    main.cpp
)

# 链接yoga库
target_link_libraries(yoga_demo
    PRIVATE
    yoga::yoga
)

# 包含目录
target_include_directories(yoga_demo PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# 设置C++标准
set_property(TARGET yoga_demo PROPERTY CXX_STANDARD 20)
set_property(TARGET yoga_demo PROPERTY CXX_STANDARD_REQUIRED ON)
