CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(skb_solver)
#SET (CMAKE_C_COMPILER    "/usr/bin/gcc") # 填写绝对路径
#SET (CMAKE_CXX_COMPILER  "/usr/bin/g++") # 填写绝对路径
SET(CMAKE_BUILD_TYPE "Release")

SET(PRO_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PRO_INCLUDE_PATH})

SET(OUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
SET(EXECUTABLE_OUTPUT_PATH ${OUT_PATH}) 

FILE(GLOB_RECURSE MY_SRC src/*.cpp)
SET(OUT_NAME soboban_solver)
ADD_EXECUTABLE(${OUT_NAME} ${MAIN_SRC} ${MY_SRC})
