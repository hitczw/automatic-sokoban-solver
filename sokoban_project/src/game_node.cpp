#include "game_node.h"
#include "mazesolver.h"
#include "constant.h"

using namespace constant;
using namespace std;

game_node::game_node(set<point>& bxp,point& ps) {
    box_list = bxp;
    person_point = ps;
}

game_node::game_node(){}

void game_node::get_matrix0(vector<vector<char>>& result)const {
    result = blank_matrix;
    for (auto item = box_list.begin(); item != box_list.end(); item++) {
        result[(*item).x][(*item).y] = BOX;
    }
}

vector<vector<char>> game_node::get_matrix()const {
    auto result = blank_matrix;

    for (auto item = box_list.begin(); item != box_list.end(); item++) {
        result[(*item).x][(*item).y] = BOX;
    }
    return result;
}

vector<vector<char>> game_node::get_matrix2()const {
    vector<vector<char>> result;
    result = blank_matrix;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (end_vec[i][j] == true) {
                result[i][j] = FINAL;
            }
        }
    }
    for (auto item_box = box_list.begin();item_box != box_list.end();item_box++) {
        end_vec[(*item_box).x][(*item_box).y]?
         result[(*item_box).x][(*item_box).y] = REDBOX:
         result[(*item_box).x][(*item_box).y] = BOX;
    }
    
    end_vec[person_point.x][person_point.y]?
     result[person_point.x][person_point.y]=PERSONF:
     result[person_point.x][person_point.y]=PERSON;
    return result;
}

bool game_node::operator==(const game_node &a)const {
    if (a.box_list == box_list) {
        vector<vector<char>> temp_matrix2;
        get_matrix0(temp_matrix2);
        maze_solver<Method::a_star, bool> maze;
        return maze.solve(temp_matrix2, a.person_point, person_point);
    }
    return false;
}

bool game_node::game_over() const {
    for (auto i = box_list.begin(); i != box_list.end(); i++) {
        auto p = *i;
        if (end_vec[p.x][p.y] == false) {return false;}
    }
    return true;
}

void game_node::get_moved(const point& box_before, point& box_new,game_node* result) const {
    *result = *this;
    auto item = result->box_list.find(box_before);
    result->box_list.erase(item);
    result->box_list.insert(box_new);
    result->person_point = box_before;
}
