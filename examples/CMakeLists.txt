# 示例程序的CMakeLists.txt

# GLFW示例
add_executable(glfw_hello
    glfw_hello/base_win.cpp
)

# 多窗口示例
add_executable(multi_win
    glfw_hello/multi_win.cpp
)

# 链接GLFW和OpenGL库
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

# 包含目录
target_include_directories(glfw_hello PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_include_directories(multi_win PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)
