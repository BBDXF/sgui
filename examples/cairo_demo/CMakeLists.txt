# Cairo Demo CMakeLists.txt

# 添加可执行文件
add_executable(cairo_demo main.cpp)

# 包含头文件目录
target_include_directories(cairo_demo PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ref/yoga
    ${CAIRO_INCLUDE_DIRS}
)

# 链接库
target_link_libraries(cairo_demo PRIVATE
    sgui_static  # 主SGUI库
    ${CAIRO_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

# 如果是Linux系统，需要链接X11库
if(UNIX AND NOT APPLE)
    target_link_libraries(cairo_demo PRIVATE X11)
endif()

# 设置编译属性
set_target_properties(cairo_demo PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
