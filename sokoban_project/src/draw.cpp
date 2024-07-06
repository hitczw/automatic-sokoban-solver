#include "draw.h"
#include "constant.h"
#include "mazesolver.h"
#include <iostream>
#ifdef _WIN32 
    #include <conio.h>  
#else  //Linux platform  
    #include <termios.h>  
#endif

using namespace constant;
using namespace std;

void draw_picture::draw_pic(vector<vector<char>>& matrix) {
#ifdef _WIN32
    system("cls");
#else
    printf ("\033c");
#endif
    cout<<"w for next, s for back, space for initial, q for quit" << endl;
    for (int a = 0; a < m; a++) {
        for (int b = 0; b < n; b++) {
            cout<<symbols[matrix[a][b]];
        }
        cout<<endl;
    }
    cout<<endl;
}

point draw_picture::get_end(game_node& first, game_node& second) {
    auto s_box = second.box_list;
    auto f_box = first.box_list;
    point p;
    for (auto s = s_box.begin(); s != s_box.end(); s++) {
        if (f_box.find(*s) == f_box.end()) {
            p = *s;
            break;
        }
    }
    return second.person_point * 2 - p;
}

vector<game_node> draw_picture::get_complete(const vector<game_node>& input) {
    vector<game_node> output;
    output.push_back(input[0]);
    maze_solver maze;
    for (int i = 0; i < input.size() - 1; i++) {
        auto first = input[i + 1];
        auto second = input[i];
        auto end_p = get_end(first, second);
        auto path = maze.get_pace(first.get_matrix(), end_p, first.person_point);

        for (int j = 0; j < path.size(); j++) {
            first.person_point = path[j];
            output.push_back(first);
        }
    }

    return output;
}

draw_picture::draw_picture() {
    symbols[WALL] = WALL_CHAR;
    symbols[FINAL] = FINAL_CHAR;
    symbols[BLANK] = BLANK_CHAR;
    symbols[BOX] = BOX_CHAR;
    symbols[REDBOX] = REDBOX_CHAR;
    symbols[PERSON] = PERSON_CHAR;
    symbols[PERSONF] = PERSONF_CHAR;
}

void draw_picture::draw(vector<game_node>& sss) {

    auto ss = get_complete(sss);
    int i = ss.size() - 1;
    auto matrix = ss[i].get_matrix2();
    draw_pic(matrix);
    char key;

    while (1) {

#ifdef _WIN32  
        key = getch();  
#else  
        struct termios stored_settings;
        struct termios new_settings;
        tcgetattr (0, &stored_settings);
        new_settings = stored_settings;
        new_settings.c_lflag &= (~ICANON);
        new_settings.c_cc[VTIME] = 0;
        new_settings.c_cc[VMIN] = 1;
        tcsetattr (0, TCSANOW, &new_settings);
        key = getchar();
        putchar('\b');
        tcsetattr (0, TCSANOW, &stored_settings);
#endif  
    
        if (key == 'w') {
            if(i == 0){continue;}
            i = i - 1;
            matrix = ss[i].get_matrix2();
            draw_pic(matrix);
        }
        else if (key == 's') {
            if(i == ss.size() - 1){continue;}
            i = i+1;
            matrix = ss[i].get_matrix2();
            draw_pic(matrix);
        }
        else if (key == ' ') {
            i = ss.size() - 1;
            matrix = ss[i].get_matrix2();
            draw_pic(matrix);
        }
        else if (key == 'q'){
            break;
        }
    }
}
