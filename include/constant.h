#pragma once

#include "point.h"
#include "my_memory.h"
#include <vector>
#include <cstdint>

enum class Method {
    bfs,
    dfs,
    a_star
};

namespace constant {
    extern int8_t m;
    extern int8_t n;
    extern const point four_direction[4];
    extern std::vector<std::vector<char>> blank_matrix;
    extern std::vector<std::vector<bool>> end_vec;
    extern std::vector<std::vector<bool>> matrix0;
    extern my_memory_pool maze_mp;
    enum game_items {WALL = 0, FINAL, BLANK, BOX, REDBOX, PERSON, PERSONF};
    enum game_items_char {WALL_CHAR    = '#',
                          FINAL_CHAR   = '.',
                          BLANK_CHAR   = ' ',
                          BOX_CHAR     = '$',
                          REDBOX_CHAR  = '*',
                          PERSON_CHAR  = '@',
                          PERSONF_CHAR = '+'
                         };
    bool is_inside(const point &p);
}