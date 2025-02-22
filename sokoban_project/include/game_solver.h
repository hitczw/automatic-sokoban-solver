#pragma once

#include "game_node.h"
#include "repeat.h"
#include "locked.h"
#include "my_memory.h"
#include <string>
#include <functional>

class detect_legal {
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

private:
    game_node init;
    repeat rpt;
    locked lk;
    my_memory_pool game_mem;
    std::function<void(const game_node*, std::function<void(const game_node*)>)> get_neighbors;
    std::function<bool(const game_node*)> is_visited;
    std::function<void(const game_node*)> mark_visited;
    std::function<bool(const game_node*, const game_node*)> is_equal;

    int get_nums2(game_node input);
    void set_lambda_function();
    std::vector<point> get_legal_point(std::vector<std::vector<char>>& vec, point p);
    std::vector<std::vector<int>> Astar_init();
    void vars_init(game_node& input);
    void vars_clear(game_node& input);

public:
    game_solver(std::string& game_map, unsigned int mm, unsigned int nn, int memval);
    std::vector<game_node> test_template(int x);
};