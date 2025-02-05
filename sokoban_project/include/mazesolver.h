#pragma once

#include "point.h"
#include "constant.h"
#include "solver_template.h"
#include <vector>
#include <functional>

template<Method tmethod, typename Return_Type>
class maze_solver {
private:
    std::vector<std::vector<bool>> zero_matrix;//todo: need check

    const std::vector<std::vector<char>>* grid;//todo: need comfirm
public:

    maze_solver(){}

    Return_Type solve(const std::vector<std::vector<char>>& _grid, const point& start, const point& end)
    {
        auto is_visited = [&](const point* n) -> bool {
            return zero_matrix[n->x][n->y] == true;
        };

        auto mark_visited = [&](const point* n) {
            zero_matrix[n->x][n->y] = true;
        };

        auto get_neighbors = [&](const point* n, std::function<void(const point*)> callback) {
            std::vector<point*> points = {
                new point(n->x + 1, n->y),
                new point(n->x, n->y + 1),
                new point(n->x - 1, n->y),
                new point(n->x, n->y - 1)
            };

            for (const point* p : points) {
                if (constant::is_inside(*p) and ((*grid)[p->x][p->y] == constant::BLANK)){
                    callback(p);
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
        return usolver.solve(&start, &end, get_neighbors, is_visited, mark_visited, is_equal, heuristic);
    }
};