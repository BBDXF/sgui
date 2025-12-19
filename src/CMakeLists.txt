# SGUI库的CMakeLists.txt

# 设置CMake策略
cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0072 NEW)

# 设置库名称
set(SGUI_LIB_NAME sgui)

# 找到所需的依赖库
find_package(glfw3 REQUIRED)

# 查找X11库（Linux平台需要）
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
endif()

# 收集所有源文件
file(GLOB SGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/window/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/controls/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/internal/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/layout/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/render/*.cpp"
)

# 创建静态库
add_library(${SGUI_LIB_NAME}_static STATIC ${SGUI_SOURCES})

# 创建动态库
add_library(${SGUI_LIB_NAME}_shared SHARED ${SGUI_SOURCES})

# 创建别名库，方便用户选择使用静态或动态库
add_library(${SGUI_LIB_NAME}::${SGUI_LIB_NAME} ALIAS ${SGUI_LIB_NAME}_static)

# 设置库的包含目录
target_include_directories(${SGUI_LIB_NAME}_static
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(${SGUI_LIB_NAME}_shared
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接依赖库
target_link_libraries(${SGUI_LIB_NAME}_static
    PUBLIC
    yoga::yoga
    PRIVATE
    glfw
    ${CAIRO_LIBRARIES}
)

target_link_libraries(${SGUI_LIB_NAME}_shared
    PUBLIC
    yoga::yoga
    PRIVATE
    glfw
    ${CAIRO_LIBRARIES}
)

# 在Linux平台上链接X11库
if(UNIX AND NOT APPLE AND X11_FOUND)
    target_link_libraries(${SGUI_LIB_NAME}_static PRIVATE ${X11_LIBRARIES})
    target_link_libraries(${SGUI_LIB_NAME}_shared PRIVATE ${X11_LIBRARIES})
    target_include_directories(${SGUI_LIB_NAME}_static PRIVATE ${X11_INCLUDE_DIR})
    target_include_directories(${SGUI_LIB_NAME}_shared PRIVATE ${X11_INCLUDE_DIR})
endif()

# 添加Cairo包含目录
target_include_directories(${SGUI_LIB_NAME}_static
    PRIVATE
    ${CAIRO_INCLUDE_DIRS}
)

target_include_directories(${SGUI_LIB_NAME}_shared
    PRIVATE
    ${CAIRO_INCLUDE_DIRS}
)

# 添加Cairo编译选项
target_compile_options(${SGUI_LIB_NAME}_static
    PRIVATE
    ${CAIRO_CFLAGS_OTHER}
)

target_compile_options(${SGUI_LIB_NAME}_shared
    PRIVATE
    ${CAIRO_CFLAGS_OTHER}
)

# 设置输出目录
# set_target_properties(${SGUI_LIB_NAME}_static PROPERTIES
#     ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
#     LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
# )

# set_target_properties(${SGUI_LIB_NAME}_shared PROPERTIES
#     LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
#     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
# )

# 设置版本和SO版本信息（仅适用于动态库）
# 使用项目版本来保持一致性
set_target_properties(${SGUI_LIB_NAME}_shared PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# 安装规则
install(TARGETS ${SGUI_LIB_NAME}_static ${SGUI_LIB_NAME}_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/yoga
    DESTINATION include/yoga
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

# 导出配置文件（可选，用于find_package）
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/SGUIConfig.cmake.in
              ${CMAKE_BINARY_DIR}/SGUIConfig.cmake @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/SGUIConfig.cmake
    DESTINATION lib/cmake/sgui)

# # 在构建过程中复制头文件到构建目录，方便开发和测试
# # 创建include目录
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)

# # 复制所有头文件到构建目录
# file(GLOB SGUI_HEADERS "${PROJECT_SOURCE_DIR}/include/*.h" "${PROJECT_SOURCE_DIR}/include/*.hpp")
# foreach(header_file ${SGUI_HEADERS})
#     get_filename_component(header_name ${header_file} NAME)
#     configure_file(${header_file} ${CMAKE_BINARY_DIR}/include/${header_name} COPYONLY)
# endforeach()

# # 添加一个自定义目标来确保头文件被复制
# add_custom_target(copy_headers ALL
#     COMMENT "Copying SGUI headers to build directory"
#     DEPENDS ${SGUI_HEADERS}
# )

# # 确保头文件复制在库构建之前完成
# add_dependencies(${SGUI_LIB_NAME}_static copy_headers)
# add_dependencies(${SGUI_LIB_NAME}_shared copy_headers)
