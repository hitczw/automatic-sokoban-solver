#pragma once

#include "point.h"
#include <vector>

enum class Method {
    bfs,
    dfs,
    a_star
};

namespace constant {
    extern char m;
    extern char n;
    extern const point four_direction[4];
    extern std::vector<std::vector<char>> blank_matrix;
    extern std::vector<std::vector<bool>> end_vec;
    extern std::vector<std::vector<bool>> matrix0;

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