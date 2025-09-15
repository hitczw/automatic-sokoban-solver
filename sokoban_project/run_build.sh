#!/bin/bash

# 设置默认编译目标
if [ "$1" = "w" ]; then
    TARGET="windows"
    echo "编译目标: Windows (交叉编译)"
elif [ "$1" = "r" ]; then
    TARGET="raspberry"
    echo "编译目标: 树莓派 (交叉编译)"
else
    TARGET="native"
    echo "编译目标: 本机平台"
fi

# 清理并创建构建目录
rm -rf build
mkdir build
cd build

# 根据目标平台运行 CMake
if [ "$TARGET" = "windows" ]; then
    cmake -DTARGET_OS=windows ..
elif [ "$TARGET" = "raspberry" ]; then
    cmake -DTARGET_OS=linux ..
else
    # 本机编译，不设置交叉编译工具链
    cmake ..
fi

# 编译拷贝
make -j4
cp ../resource/box.txt ./

# 复制box.txt
if [ "$TARGET" = "windows" ]; then
    echo "编译完成: sokoban_solver.exe"
else
    echo "编译完成: sokoban_solver"
fi