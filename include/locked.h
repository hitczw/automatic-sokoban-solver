#pragma once

#include "point.h"
#include <vector>

class locked {
private:
    std::vector<std::vector<bool>> side_point;
    bool locked_double(std::vector<std::vector<char>>& matrix_with_box, point& box, point& wall);
    std::vector<point> get_box_wall(point &box);
    bool is_next_two_wall(std::vector<point> &around);
public:
    locked();
    void init();
    bool is_locked(point& box, std::vector<std::vector<char>>& matrix_with_box);
};
