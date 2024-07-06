#pragma once

#include "point.h"
#include <vector>
#include <set>

class game_node {
private:

public:
    
    std::set<point> box_list;
    game_node* last_state;
    point person_point;

    game_node(std::set<point>& bxp,point& ps,game_node* last=0);

    game_node();

    void get_matrix0(std::vector<std::vector<char>>& result)const;

    std::vector<std::vector<char>> get_matrix()const;

    std::vector<std::vector<char>> get_matrix2()const;

    bool operator==(const game_node &a)const;

    bool game_over();

    void get_moved(const point& box_before, point& box_new,game_node* result);
};

