#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "game_solver.h"
#include "draw.h"

using namespace std;

void read_file(int& mm, int& nn, string& temp){
    mm=0;
    nn=0;
    temp.clear();
    ifstream file_read;
    file_read.open("box.txt",ios::in);

    if (!file_read) {
        printf("box.txt dose not exist!\n");
#ifdef _WIN32
        system("pause");
#endif
        exit(100);
    }

    string x;
    string tempr;
    while (getline(file_read,x)) {
        tempr += x;
        mm += 1;
    }
    for(auto &tc: tempr){
        if(tc != '\r' && tc != '\n'){
            temp.push_back(tc);
        }
    }
    
    nn = temp.size() / mm;
    file_read.close();
}

int main() {
    int mm;
    int nn;
    string temp;
    read_file(mm,nn,temp);
    
    printf("please select your algorithm(enter 0 or 1 or 2)\n");
    printf("0: A*;    1: dfs    2: bfs\n");
    char input;
    int iinput;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (input == '0' || input == '1' || input == '2') {
        iinput = input - '0'; // 将字符转换为对应的整数
    }
    else {
        printf("wrong input!!\n");
        exit(-1);
    }

    printf("please input the memory you want to use(unit: MB)\n");
    int memval;
    std::cin >> memval;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    game_solver ga(temp, mm, nn, memval);

    auto ss = ga.test_template(iinput);

    printf("press Enter to show solves\n");

    cin.get();
    draw_picture d;
    d.draw(ss);

#ifdef _WIN32
    system("pause");
#endif
}
