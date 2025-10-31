cmake_minimum_required(VERSION 3.12)
project(SlowRSA VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# 加载第三方测试库
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party/googletest)
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party/benchmark)

option(BUILD_TEST "BUILD_TEST" ON)

# # 编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -Wall -Wextra)
else()
    add_compile_options(-O2 -DNDEBUG)
endif()

# 创建可执行文件
add_subdirectory(src)

# 测试
if (BUILD_TEST)
    enable_testing()
    add_subdirectory(tests)
endif()