#include "game_solver.h"
#include "constant.h"
#include "locked.h"
#include "repeat.h"
#include "solver_template.h"
#include <deque>
#include <cstdint>
#include <time.h>

using namespace constant;
using namespace std;

void detect_legal::get_value(const vector<vector<char>>& maze_matrix, const point& person_point) {
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

detect_legal::detect_legal(const game_node* node) {
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

game_solver::game_solver(string& game_map, unsigned int mm, unsigned int nn) {

    m = mm;
    n = nn;
    matrix0  = vector<vector<bool>>(mm, vector<bool>(nn, false));
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

    init = game_node(box_point_start,person_start);
    set_lambda_function();
}

int game_solver::get_nums2(game_node input) {
    auto p = *(input.box_list.begin());
    if (end_vec[p.x][p.y] == true) { return 0; }
    
    rpt.init(input);
    lk.init();

    int result=0;
    deque<game_node*> temp_vec;
    temp_vec.push_back(&input);
    vector<game_node*> for_del;

    Solver_template<vector<game_node>, game_node, Method::bfs> gsolver1;
    auto resx = gsolver1.solve(&input, nullptr, get_neighbors, is_visited, mark_visited, is_equal);
    if (resx.size() == 0)
    {
        return 1000;
    }
    return resx.size();
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
                int min_ = get_nums2(new_node);
                if (min_ < min_num) {
                    min_num = min_;
                }
            }
            result[i][j] = min_num;
        }
    }
    return result;
}

void game_solver::set_lambda_function(){

    is_visited = [&](const game_node* n) -> bool {
        return rpt.is_repeat2(n);
    };

    mark_visited = [&](const game_node* n) {
        rpt.insert(n);
    };

    get_neighbors = [&](const game_node* n_min, std::function<void(const game_node*)> callback) {
        detect_legal test(n_min);
        for (auto item = n_min->box_list.begin(); item != n_min->box_list.end(); item++) {
            auto box = *item;
            for (auto direction : four_direction) {
                auto new_point = box + direction;
                if (test.can_get(new_point)) {
                    if (test.can_box_move(box, new_point)) {

                        auto new_box_point = *item * 2 - new_point;
                        auto temp_box2 = new game_node();
                        n_min->get_moved(*item, new_box_point, temp_box2);
                        vector<vector<char>>temp_matrix2;
                        temp_box2->get_matrix0(temp_matrix2);

                        if (lk.is_locked(new_box_point, temp_matrix2)) {
                            //todo
                            delete temp_box2;
                        }

                        else {
                            callback(temp_box2);
                        }
                    }}}}
    };

    is_equal = [](const game_node* a, const game_node* b) -> bool {
        return a->game_over();
    };//todo:实际只用来判断终点，但终点和普通节点判断逻辑不同

}

vector<game_node> game_solver::test_template(int input){
    auto vec = Astar_init();
    rpt.init(init);
    lk.init();

    auto heuristic = [&](const game_node* a, const game_node* b) {
        int f = 0;
        for (auto i = a->box_list.begin(); i != a->box_list.end(); i++) {
            auto p = *i;
            f += vec[p.x][p.y];
        }
        return f;
    };

    Solver_template<vector<game_node>, game_node, Method::a_star> gsolver0;
    Solver_template<vector<game_node>, game_node, Method::bfs> gsolver1;
    Solver_template<vector<game_node>, game_node, Method::dfs> gsolver2;
    vector<game_node> resx;
    printf("compute start!!\n");
    auto t1 = clock();
    if (input == 0)
    {
        resx = gsolver0.solve(&init, nullptr, get_neighbors, is_visited, mark_visited, is_equal, heuristic);
    }
    else if (input == 1)
    {
        resx = gsolver1.solve(&init, nullptr, get_neighbors, is_visited, mark_visited, is_equal);
    }
    else if (input == 2)
    {
        resx = gsolver2.solve(&init, nullptr, get_neighbors, is_visited, mark_visited, is_equal);
    }
    else{}
    auto t2 = clock();
    printf("compute complete!!\n");
    printf("time cost %fs\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    printf("push box %d times\n", resx.size());
    printf("all status %d\n", rpt.zobrist_hash.size());
    return resx;
}