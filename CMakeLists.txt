CMAKE_MINIMUM_REQUIRED(VERSION 3.10.1)
project(skb_solver)

if(NOT DEFINED TARGET_OS)
    set(TARGET_OS "native")
endif()

# 根据目标平台设置交叉编译工具链
if(TARGET_OS STREQUAL "windows")
    # Windows 交叉编译设置
    SET(CMAKE_SYSTEM_NAME Windows)
    SET(CMAKE_SYSTEM_PROCESSOR x86_64)
    SET(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    SET(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
elseif(TARGET_OS STREQUAL "linux")
    # 树莓派交叉编译设置
    SET(CMAKE_SYSTEM_NAME Linux)
    SET(CMAKE_SYSTEM_PROCESSOR aarch64)
    SET(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
    SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
else()
    # 本机编译，使用系统默认编译器
endif()

SET(CMAKE_BUILD_TYPE "Release")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

FILE(GLOB_RECURSE MY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
ADD_EXECUTABLE(sokoban_solver ${MY_SRC})

target_compile_options(sokoban_solver PRIVATE -Wall -Wextra -pedantic -Werror -O3)
target_link_options(sokoban_solver PRIVATE -static -s)
