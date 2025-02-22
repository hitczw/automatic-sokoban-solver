#pragma once

#include "point.h"
#include <vector>
#include <set>

class game_node {
public:
    std::set<point> box_list;
    point person_point;
    game_node(std::set<point>& bxp,point& ps);
    game_node();
    void get_matrix0(std::vector<std::vector<char>>& result)const;
    std::vector<std::vector<char>> get_matrix()const;
    std::vector<std::vector<char>> get_matrix2()const;
    bool operator==(const game_node &a)const;
    bool game_over()const;
    void get_moved(const point& box_before, point& box_new,game_node* result)const;
};