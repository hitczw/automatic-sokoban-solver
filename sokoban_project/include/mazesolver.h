#pragma once

#include "point.h"
#include <memory>
#include <vector>

class npoint {
public:
    point p;
    std::shared_ptr<npoint> last_p;

    npoint(const point& x);
    npoint();
};

class Apoint {
public:
    point p;
    int f;
    int g;
    int h;
    Apoint* father;

    Apoint();
    bool operator<(const Apoint& b)const;
};

class maze_solver {
private:
    bool bfs_maze_solver(const std::vector<std::vector<char>> &maze_matrix, const point &end, const point& person_point);

    bool dfs_maze_solver(const std::vector<std::vector<char>> &maze_matrix, const point &end, const point& person_point);

    bool Astar(const std::vector<std::vector<char>> &maze_matrix, const point &end, const point& person_point);

public:
    typedef enum { dfs, bfs, a_star }METHOD;

    bool operator()(METHOD method, const std::vector<std::vector<char>> &maze_matrix, const point &end, const point& person_point);
    
    std::vector<point> get_pace(const std::vector<std::vector<char>> &maze_matrix, const point &end, const point& start);
};