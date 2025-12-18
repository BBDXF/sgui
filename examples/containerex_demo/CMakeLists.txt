# ContainerEx Demo CMakeLists.txt

# 添加containerex_demo可执行文件
add_executable(containerex_demo
    main.cpp
)

# 链接库
target_link_libraries(containerex_demo
    PRIVATE
    sgui_static
    yoga::yoga
)

# 包含目录
target_include_directories(containerex_demo PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# 设置C++标准
set_property(TARGET containerex_demo PROPERTY CXX_STANDARD 17)
set_property(TARGET containerex_demo PROPERTY CXX_STANDARD_REQUIRED ON)

# 设置编译选项
target_compile_options(containerex_demo PRIVATE -Wall -Wextra)
