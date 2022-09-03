# 编译使用的 cmake 最低版本
cmake_minimum_required(VERSION 3.16)

# cmake 注释
#单行注释
#[[
    多行注释
]]

# 项目名称和版本
project(studycplusplus VERSION 0.1)

include_directories(include)

# 生成的可执行文件名称和项目源文件
add_executable(cplusplus test/main.cpp)
