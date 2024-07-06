#include "game_solver.h"
#include "constant.h"
#include "locked.h"
#include "repeat.h"
#include <deque>
#include <iostream>
#include <time.h>

using namespace constant;
using namespace std;

void detect_legal::get_value(const vector<vector<char>>& maze_matrix, point& person_point) {
    deque<point> for_loop;
    for_loop.push_back(person_point);
    point new_point;

    while (!for_loop.empty()) {
        auto temp_point = for_loop.back();
        for_loop.pop_back();

        for (auto& direction : four_direction) {
            new_point = direction + temp_point;
            if (is_inside(new_point)) {
                if (value[new_point.x][new_point.y] == false) {
                    if (maze_matrix[new_point.x][new_point.y] == BLANK) {
                        value[new_point.x][new_point.y] = true;
                        for_loop.push_front(new_point);
                    }
                }
            }
        }
    }
}

detect_legal::detect_legal(game_node* node) {
    value = vector<vector<bool>>(m, vector<bool>(n, false));
    node->get_matrix0(matrix_with_box);
    get_value(matrix_with_box,node->person_point);
    value[node->person_point.x][node->person_point.y] = true;
}

detect_legal::detect_legal(vector<vector<char>>& matrix,point& start) {
    matrix_with_box = matrix;
    value = vector<vector<bool>>(m, vector<bool>(n, false));
    get_value(matrix_with_box, start);
    value[start.x][start.y] = true;
}

detect_legal::detect_legal() {}

bool detect_legal::can_get(point& des) {
    return value[des.x][des.y] == true;
}

bool detect_legal::can_box_move(point& box, point& person) {
    point new_point;
    new_point = box * 2 - person;
    return matrix_with_box[new_point.x][new_point.y] == BLANK;
}


Astar_node::Astar_node() {
    f = 0;
    g = 0;
    h = 0;
}

Astar_node::Astar_node(game_node& c):game_node(c) {
    f = 0;
    g = 0;
    h = 0;
}

void Astar_node::get_f(vector<vector<int>>& vec) {
    f = 0;
    for (auto i = box_list.begin(); i != box_list.end(); i++) {
        auto p = *i;
        f += vec[p.x][p.y];
    }
}

bool Astar_node::operator<(const Astar_node& b)const{
    return f < b.f;
}

game_solver::game_solver(string& game_map, unsigned int mm, unsigned int nn) {

    m = mm;
    n = nn;
    matrix0  =       vector<vector<bool>>(mm, vector<bool>(nn, false));
    end_vec = vector<vector<bool>>(mm, vector<bool>(nn, false));
    blank_matrix = vector<vector<char>>(mm, vector<char>(nn, 0));

    point person_start;
    set<point> box_point_start;

    char temp_c;
    point temp_p;

    bool up = true;
    bool down = true;

    bool has_blank = false;
    bool has_end = false;

    for (char x = 0; x < m; x++) {
        for (char y = 0; y < n; y++) {
            temp_c = game_map[x*n + y];
            temp_p = { x,y };
            switch (temp_c) {
            case '#':
                blank_matrix[x][y] = WALL;
                break;
            case ' ':
                blank_matrix[x][y] = BLANK;
                break;
            case '$':
                blank_matrix[x][y] = BLANK;
                box_point_start.insert(temp_p);
                break;
            case '*':
                blank_matrix[x][y] = BLANK;
                box_point_start.insert(temp_p);
                end_vec[x][y] = true;
                break;
            case '.':
                blank_matrix[x][y] = BLANK;
                end_vec[x][y] = true;
                break;
            case '@':
                blank_matrix[x][y] = BLANK;
                person_start = temp_p;
                break;
            case '+':
                blank_matrix[x][y] = BLANK;
                person_start = temp_p;
                end_vec[x][y] = true;
                break;
            default:
                break;
            }
        }
    }

    init = game_node(box_point_start,person_start,0);
}

int game_solver::get_nums(game_node input) {
    auto p = *(input.box_list.begin());
    if (end_vec[p.x][p.y] == true) { return 0; }
    locked lc(input);
    repeat rp(input);
    int result=0;
    deque<game_node*> temp_vec;
    temp_vec.push_back(&input);
    vector<game_node*> for_del;

    while (!temp_vec.empty()) {
        game_node* node = temp_vec.back();
        temp_vec.pop_back();
        detect_legal  test(node);
        for (auto item = node->box_list.begin(); item != node->box_list.end(); item++) {
            auto box = *item;
            for (auto direction : four_direction) {
                auto new_point = box + direction;
                if (test.can_get(new_point)) {
                    if (test.can_box_move(box, new_point)) {

                        auto new_box_point = *item * 2 - new_point;
                        auto temp_box2 = new game_node();
                        node->get_moved(*item, new_box_point, temp_box2);
                        vector<vector<char>>temp_matrix2;
                        temp_box2->get_matrix0(temp_matrix2);
                        temp_box2->last_state = node;

                        if (lc.is_locked(new_box_point, temp_matrix2) || rp.is_repeat(temp_box2)) {
                            delete temp_box2;
                        }

                        else if (temp_box2->game_over()) {
                            while (temp_box2) {
                                result += 1;
                                temp_box2 = temp_box2->last_state;
                            }

                            for (auto &x : for_del) { delete x; }
                            return result-1;
                        }

                        else {
                            temp_vec.push_front(temp_box2);
                            for_del.push_back(temp_box2);
                        }
                    }
                }
            }
        }
    }
    result = 1000;
    return result;
}

vector<point> game_solver::get_legal_point(vector<vector<char>>& vec, point p) {
    vector<point> result;
    vector<detect_legal> detect;
    bool flag = false;
    for (auto i = 0; i < 4; i++) {
        auto pp = p + four_direction[i];
        if (vec[pp.x][pp.y] == BLANK) {
            for (auto& dect : detect) {
                if (dect.can_get(pp)) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                flag = false;
                continue;
            }
            detect.push_back(detect_legal(vec,pp));
            result.push_back(pp);

        }
    }
    return result;
}

vector<vector<int>> game_solver::Astar_init() {
    vector<vector<int>> result(m,vector<int>(n,0));
    
    for (int i = 0; i <m; i++) {
        for (int j = 0; j < n; j++) {
            if (blank_matrix[i][j] == WALL) {
                result[i][j] = 1000;
                continue;
            }
            game_node new_node;
            new_node.box_list.insert(point(i,j));
            vector<vector<char>>vec;
            new_node.get_matrix0(vec);
            auto person_point = get_legal_point(vec,point(i,j));
            if (person_point.empty()) {
                result[i][j] = 1000;
                continue;
            }
            int min_num=INT32_MAX;
            for (auto& dd : person_point){
                new_node.person_point = dd;
                int min_ = get_nums(new_node);
                if (min_ < min_num) {
                    min_num = min_;
                }
            }
            result[i][j] = min_num;
        }
    }
    return result;
}

bool game_solver::cmp::operator()(const Astar_node* a, const Astar_node* b)const {
    return *a < *b;
}

vector<game_node> game_solver::Astar_solver() {
    auto vec = Astar_init();

    multiset<Astar_node*, cmp> open;
    repeat closed(init);
    locked lc(init);
    vector<game_node>result;
    Astar_node start_p;
    start_p = init;
    open.insert(&start_p);
    vector<Astar_node*> for_del;

    cout << "compute start!..." << endl;
    auto t1 = clock();
    while (!open.empty()) {
        auto n_min = *(open.begin());
        open.erase(open.begin());
        detect_legal  test(n_min);

        for (auto item = n_min->box_list.begin(); item != n_min->box_list.end(); item++) {
            auto box = *item;
            for (auto direction : four_direction) {
                auto new_point = box + direction;
                if (test.can_get(new_point)) {
                    if (test.can_box_move(box, new_point)) {

                        auto new_box_point = *item * 2 - new_point;
                        auto temp_box2 = new Astar_node();
                        n_min->get_moved(*item, new_box_point, temp_box2);
                        vector<vector<char>>temp_matrix2;
                        temp_box2->get_matrix0(temp_matrix2);
                        temp_box2->last_state = n_min;

                        if (lc.is_locked(new_box_point, temp_matrix2) || closed.is_repeat((temp_box2))) {
                            delete temp_box2;
                        }

                        else if (temp_box2->game_over()) {
                            cout << "compute complete!" << endl;
                            auto t2 = clock();
                            cout << "time cost " << (double)( t2-t1)/CLOCKS_PER_SEC<< "s" << endl;
                            auto nd = (game_node*)(temp_box2);

                            while (nd) {
                                result.push_back(*nd);
                                nd = nd->last_state;
                            }

                            for (auto x : for_del) { delete x; }
                            cout << "all status " << closed.zobrist_hash.size() << endl;
                            cout << "push box times " << result.size() - 1 << endl;
                            return result;
                        }

                        else {
                            temp_box2->get_f(vec);
                            open.insert(temp_box2);
                            for_del.push_back(temp_box2);
                        }
                    }
                }
            }
        }
    }
    cout << "unsolveble" << endl;
    exit(1000);
}
