#include "locked.h"
#include "constant.h"
#include <cmath>
using namespace constant;
using namespace std;

bool locked::locked_double(vector<vector<char>>& matrix_with_box, point& box, point& wall) {
    auto bx = box.x;
    auto by = box.y;
    auto wx = wall.x;
    auto wy = wall.y;
    if (bx == wx) {
        auto box_down = matrix_with_box[bx + 1][by];
        auto box_up = matrix_with_box[bx - 1][by];
        auto wall_down = matrix_with_box[wx + 1][wy];
        auto wall_up = matrix_with_box[wx - 1][wy];
        auto wall_down_op = matrix_with_box[wx + 1][2 * by - wy];
        auto wall_up_op = matrix_with_box[wx - 1][2 * by - wy];

        return (box_up == BOX && (wall_up == WALL || wall_up_op == WALL || wall_up == BOX)) || \
               (box_down == BOX && (wall_down == WALL || wall_down_op == WALL || wall_down == BOX));
    }
    else if (by == wy) {
        auto box_right = matrix_with_box[bx][by + 1];
        auto box_left = matrix_with_box[bx][by - 1];
        auto wall_right = matrix_with_box[wx][wy + 1];
        auto wall_left = matrix_with_box[wx][wy - 1];
        auto wall_right_op = matrix_with_box[2 * bx - wx][wy + 1];
        auto wall_left_op = matrix_with_box[2 * bx - wx][wy - 1];

        return (box_right == BOX && (wall_right == WALL || wall_right_op == WALL || wall_right == BOX)) || \
               (box_left == BOX && (wall_left == WALL || wall_left_op == WALL || wall_left == BOX));
    }
    return false;
}

vector<point> locked::get_box_wall(point &box) {
    vector<point> result;
    point new_point;
    for (auto direction : four_direction) {
        new_point = direction + box;
        if (is_inside(new_point)) {
            if (blank_matrix[new_point.x][new_point.y] == WALL) {
                result.push_back(new_point);
            }
        }
    }
    return result;
}

bool locked::is_next_two_wall(vector<point> &around) {
    if (around.size() <= 1) {
        return false;
    }
    if (around.size() >= 3) {
        return true;
    }

    return abs(around[0].x - around[1].x) == 1;
}

locked::locked(game_node& init) {
    side_point = matrix0;
    side_point[0][0] = true;
    side_point[0][n - 1] = true;
    side_point[m - 1][0] = true;
    side_point[m - 1][n - 1] = true;
    bool up_down_lock = true;

    bool has_blank = false;
    bool has_end = false;

    char last_j = 1;
    char last_i = 1;

    for (char i = 1; i < m - 1; i++) {
        for (char j = 0; j < n; j++) {
            if (blank_matrix[i][j] == WALL) {
                side_point[i][j] = true;
                if (has_blank && up_down_lock && (!has_end)) {
                    for (char k = last_j; k < j; k++) {
                        side_point[i][k] = true;
                    }
                }
                up_down_lock = true;
                has_blank = false;
                has_end = false;
                continue;
            }

            if (blank_matrix[i][j] == BLANK) {
                if (!has_blank) {
                    last_j = j;
                    has_blank = true;
                }
                if (end_vec[i][j]==true) {
                    has_end = true;
                }
            }
            if(blank_matrix[i-1][j] == BLANK && blank_matrix[i+1][j] ==BLANK){
                up_down_lock = false;
            }
        }
        up_down_lock = true;
        has_blank = false;
        has_end = false;
    }

    for (char j = 1; j < n - 1; j++) {
        for (char i = 0; i < m; i++) {
            if (blank_matrix[i][j] == WALL) {
                side_point[i][j] = true;
                if (has_blank && up_down_lock && (!has_end)) {
                    for (char k = last_i; k < i; k++) {
                        side_point[k][j] = true;
                    }
                }
                up_down_lock = true;
                has_blank = false;
                has_end = false;
                continue;
            }

            if (blank_matrix[i][j] == BLANK) {
                if (!has_blank) {
                    last_i = i;
                    has_blank = true;
                }

                if (end_vec[i][j] == true) {
                    has_end = true;
                }
            }
            if (blank_matrix[i][j-1] == BLANK && blank_matrix[i][j+1] == BLANK) {
                up_down_lock = false;
            }
        }
        up_down_lock = true;
        has_blank = false;
        has_end = false;
    }
    for (char i = 0; i < m; i++) {
        for (char j = 0; j < n; j++) {
            if (blank_matrix[i][j] == BLANK) {
                auto box = point(i, j);
                auto around = get_box_wall(box);
                if (around.empty()) {
                    continue;
                }

                if (is_next_two_wall(around)) {
                    if (end_vec[i][j] == false) {
                        side_point[box.x][box.y] = true;
                    }
                }
            }
        }
    }
}

bool locked::is_locked(point& box, vector<vector<char>>& matrix_with_box) {
    if (side_point[box.x][box.y] == true) { return true; }
    if (end_vec[box.x][box.y] == true) {
        return false;
    }

    auto around = get_box_wall(box);
    if (around.empty()) { return false; }
    return locked_double(matrix_with_box, box, around[0]);
}