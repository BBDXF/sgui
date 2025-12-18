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

# 测试输入事件
add_executable(test_input
    glfw_hello/test_input.cpp
)

# 窗口管理库demo
add_executable(repo_test
    repo_test/main.cpp
)

# libcss和libdom demo
add_executable(dom_hello
    dom_hello/main.cpp
)

# Yoga layout demo
add_subdirectory(yoga_demo)

# Container class demo
add_subdirectory(container_demo)

# ContainerEx class demo
add_subdirectory(containerex_demo)

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

target_link_libraries(repo_test
    PRIVATE
    glfw
    ${OPENGL_LIBRARIES}
    sgui_static  # 使用静态库
)

target_link_libraries(test_input
    PRIVATE
    glfw
    ${OPENGL_LIBRARIES}
    sgui_static  # 使用静态库
)

# 包含目录 - 只需要include目录，因为库已经处理了内部依赖
target_include_directories(glfw_hello PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(multi_win PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(repo_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(test_input PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# dom_hello项目 - 只需要标准C++库
target_include_directories(dom_hello PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
