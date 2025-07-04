cmake_minimum_required(VERSION 3.30)

# 设置项目名称
project(cchan)

# 查找源文件
file(GLOB_RECURSE SOURCES src/*.c)

# 添加编译选项
set(CMAKE_C_STANDARD 17)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/lib/uthash)
