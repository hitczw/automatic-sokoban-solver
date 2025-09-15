#pragma once

#include "point.h"
#include "constant.h"
#include "solver_template.h"
#include "my_memory.h"
#include <vector>
#include <functional>
#include <cstdio>

template<Method tmethod, typename Return_Type>
class maze_solver {
private:
    const std::vector<std::vector<char>>* grid;//todo: need comfirm
public:
    std::vector<std::vector<bool>> zero_matrix;//todo: need check
    maze_solver(){}
    Return_Type solve(const std::vector<std::vector<char>>& _grid, const point& start, const point& end)
    {
        auto is_visited = [&](const point*) -> bool {
            return false;
        };

        auto mark_visited = [&](const point* n) {
            zero_matrix[n->x][n->y] = true;
        };

        auto get_neighbors = [&](const point* n, std::function<void(const point*)> callback) {
            for (auto& direction : constant::four_direction) {
                point p = direction + *n;
                if (constant::is_inside(p) and ((*grid)[p.x][p.y] == constant::BLANK) and zero_matrix[p.x][p.y] == false){
                    point* new_point = new(constant::maze_mp.allocate()) point(p.x, p.y);
                    callback(new_point);
                }
            }
        };

        auto is_equal = [](const point* a, const point* b) ->bool {
            return *a == *b;
        };

        auto heuristic = [](const point* a, const point* b) {
            return std::abs(a->x - b->x) + std::abs(a->y - b->y);
        };

        grid = &_grid;
        zero_matrix = constant::matrix0;
        Solver_template<Return_Type, point, tmethod> usolver;
        constant::maze_mp.clear();
        return usolver.solve(&start, &end, get_neighbors, is_visited, mark_visited, is_equal, heuristic);
    }
};