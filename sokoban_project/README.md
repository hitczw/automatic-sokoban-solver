# automatic-sokoban-solver
推箱子自动求解程序
演示视频 bilibili av76908171;
作者hitczw

How to compile?

    cd build
    cmake ..
    make


How to run?

    ./soboban_solver

Rules?

    Enter the game information in box.txt. The file name cannot be changed
    In file box.txt
    # for wall
      for blank
    $ for box
    * for box in end
    . for end
    @ for person
    + for person in end
    After information input, save and open the software to start solving
    
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



