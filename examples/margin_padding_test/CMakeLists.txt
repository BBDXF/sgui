# Margin & Padding 测试程序

# 添加可执行文件
add_executable(margin_padding_test main.cpp)

# 链接SGUI库
target_link_libraries(margin_padding_test 
    PRIVATE 
    sgui_static
    ${CAIRO_LIBRARIES}
    ${YOGA_LIBRARIES}
)

# 包含头文件目录
target_include_directories(margin_padding_test 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CAIRO_INCLUDE_DIRS}
    ${YOGA_INCLUDE_DIRS}
)

# 编译器特定设置
if(MSVC)
    target_compile_options(margin_padding_test PRIVATE /W4)
else()
    target_compile_options(margin_padding_test PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 设置输出目录
set_target_properties(margin_padding_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
