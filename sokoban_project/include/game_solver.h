#pragma once

#include "game_node.h"
#include <string>

class detect_legal {
private:
    void get_value(const std::vector<std::vector<char>>& maze_matrix, const point& person_point);

public:
    std::vector<std::vector<bool>> value;
    std::vector<std::vector<char>> matrix_with_box;

    detect_legal(const game_node* node);

    detect_legal(std::vector<std::vector<char>>& matrix,point& start);

    detect_legal();

    bool can_get(point& des);

    bool can_box_move(point& box, point& person);
};

class game_solver {

public:

    game_node init;

    game_solver(std::string& game_map, unsigned int mm, unsigned int nn);
    
    int get_nums(game_node input);

    std::vector<point> get_legal_point(std::vector<std::vector<char>>& vec, point p);

    std::vector<std::vector<int>> Astar_init();

    std::vector<game_node> test_template(int x);
};