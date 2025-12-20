# 示例程序的CMakeLists.txt
cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0072 NEW)

# GLFW示例
add_executable(glfw_hello
    glfw_hello/base_win.cpp
)

# 多窗口示例
add_executable(multi_win
    glfw_hello/multi_win.cpp
)

# Yoga layout demo
add_subdirectory(yoga_demo)

# ContainerEx class demo
add_subdirectory(containerex_demo)

# Cairo demo
add_subdirectory(cairo_demo)

# 链接GLFW、OpenGL库和sgui库
find_package(OpenGL REQUIRED)
target_link_libraries(glfw_hello
    PRIVATE
    glfw
    ${OPENGL_LIBRARIES}
)

target_link_libraries(multi_win
    PRIVATE
    glfw
    ${OPENGL_LIBRARIES}
)

# 包含目录 - 只需要include目录，因为库已经处理了内部依赖
target_include_directories(glfw_hello PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(multi_win PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

