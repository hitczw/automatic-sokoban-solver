# automatic-sokoban-solver

202502更新：

    1.用120行算法模板，实现ASTAR,DFS,BFS三种算法，用于迷宫求解和游戏求解，减少大量重复代码
    2.交互增加用户算法选择
    3.增加内存管理, 需要自己手动输入要使用多少MB内存（项目自带实例建议10MB以上内存）

其它说明：

    1.三种算法求解速度一般ASTAR>DFS>BFS，BFS可以找到最小箱子推动次数解法
    2.求解完成后按Enter展示具体解法, w键下一步, s键上一步, space回到起点, q退出


推箱子自动求解程序
演示视频 bilibili av76908171;
作者hitczw

如何构建?

构建方法1（运行命令）：

    mkdir build
    cd build
    cmake ..
    make

构建方法2（运行脚本）：

    ./run_build.sh #编译本机应用，linux系统上运行
    ./run_build.sh w #交叉编译windows应用，windows系统运行
    ./run_build.sh r #交叉编译arm应用，可以在树莓派5上运行

如何运行？

    进入build目录，sokoban_solver是可执行程序，box.txt存储关卡信息，需要与sokoban_solver同一目录，在resource文件夹中有示例的box.txt存储关卡信息，需要与sokoban_solver同一目录，在resource文件夹中有示例的box

规则?
    
    在box.txt输入游戏信息,文件名称不能更改
    在box.txt文件中
    #表示墙
     表示空地
    $表示箱子
    *表示箱子在目的地
    .表示目的地
    @表示人
    +表示人在目的地
    信息输入完成后保存打开软件开始求解
