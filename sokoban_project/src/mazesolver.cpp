#include "mazesolver.h"
#include "constant.h"
#include <deque>
#include <set>
using namespace constant;
using namespace std;

npoint::npoint(const point& x) {p = x;}

npoint::npoint() {}


Apoint::Apoint() {
    f = 0;
    g = 0;
    h = 0;
    father = 0;
}

bool Apoint::operator<(const Apoint& b)const {
    if (f != b.f) { return f < b.f; }
    return p<b.p;
}


bool maze_solver::bfs_maze_solver(const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
    auto zero_matrix = matrix0;
    deque<point> for_loop;
    for_loop.push_back(person_point);

    point new_point;

    while (!for_loop.empty()) {
        auto temp_point = for_loop.back();
        for_loop.pop_back();

        for (auto& direction : four_direction) {
            new_point = direction + temp_point;
            if (is_inside(new_point)) {
                if (zero_matrix[new_point.x][new_point.y] == false) {
                    if (maze_matrix[new_point.x][new_point.y] == BLANK) {
                        if (new_point == end) {
                            return true;
                        }
                        zero_matrix[new_point.x][new_point.y] = true;
                        for_loop.push_front(new_point);
                    }
                }
            }
        }
    }
    return false;
}

bool maze_solver::dfs_maze_solver(const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
    auto zero_matrix = matrix0;
    vector<point> for_loop;

    bool push = false;

    for_loop.push_back(person_point);
    point new_point;

    while (true) {

        auto temp_point = for_loop.back();
        for (auto& direction : four_direction) {
            new_point = direction + temp_point;
            if (is_inside(new_point)) {
                if (zero_matrix[new_point.x][new_point.y] == false) {
                    if (maze_matrix[new_point.x][new_point.y] == BLANK) {
                        if (new_point == end) {
                            return true;
                        }
                        zero_matrix[new_point.x][new_point.y] = true;
                        for_loop.push_back(new_point);
                        push = true;
                        break;
                    }
                }
            }
        }

        if (for_loop.empty()) {
            return false;
        }

        if (!push) {
            for_loop.pop_back();
        }

        push = false;
    }
}

bool maze_solver::Astar(const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
    multiset<Apoint> open;
    auto closed     =matrix0;
    auto open_mat=matrix0;

    Apoint start_p;
    start_p.p = person_point;
    start_p.h = person_point.get_distance(end);
    start_p.f = start_p.h;

    open.insert(start_p);
    open_mat[start_p.p.x][start_p.p.y] = true;
    while (!open.empty()) {
        auto n_min = *(open.begin());
        open.erase(open.begin());
        open_mat[n_min.p.x][n_min.p.y] = false;
        closed[n_min.p.x][n_min.p.y] = true;
        for (auto& direction : four_direction) {

            auto new_point = n_min;
            new_point.p = new_point.p + direction;
            new_point.h = new_point.p.get_distance(end);
            new_point.f = new_point.g + new_point.h;

            if (is_inside(new_point.p)) {
                if (closed[new_point.p.x][new_point.p.y] == false) {
                    if (maze_matrix[new_point.p.x][new_point.p.y] == BLANK) {
                        if (new_point.p == end) {
                            return true;
                        }
                        if (open_mat[new_point.p.x][new_point.p.y] == false) {
                            open_mat[new_point.p.x][new_point.p.y] = true;
                            open.insert(new_point);
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool maze_solver::operator()(METHOD method, const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
    if (maze_matrix[end.x][end.y] != BLANK) {
        return false;
    }
    else if (maze_matrix[person_point.x][person_point.y] != BLANK) {
        return false;
    }
    else if (person_point == end) {
        return true;
    }
    switch (method) {
    case bfs:
        return bfs_maze_solver(maze_matrix, end, person_point);
    case dfs:
        return dfs_maze_solver(maze_matrix, end, person_point);
    case a_star:
        return Astar(maze_matrix, end, person_point);
    default:
        break;
    }
    return false;
}

vector<point> maze_solver::get_pace(const vector<vector<char>> &maze_matrix, const point &end, const point& start) {
    if (start == end) {
        vector<point> result;
        result.push_back(start);
        return result;
    }
    auto start_p = make_shared<npoint>(start);

    auto zero_matrix = matrix0;

    vector<shared_ptr<npoint>> for_loop;
    vector<shared_ptr<npoint>> new_loop;
    for_loop.push_back(start_p);


    while (true) {
        for (auto& temp_point : for_loop) {
            for (auto& direction : four_direction) {

                auto new_point = make_shared<npoint>();

                new_point->p = direction + temp_point->p;
                if (is_inside(new_point->p)) {
                    if (zero_matrix[new_point->p.x][new_point->p.y] == false) {
                        if (maze_matrix[new_point->p.x][new_point->p.y] == BLANK) {
                            if (new_point->p == end) {
                                new_point->last_p = temp_point;

                                vector<point> result;
                                while (new_point.use_count()) {
                                    result.push_back(new_point->p);
                                    new_point = new_point->last_p;
                                }
                                return result;
                            }
                            zero_matrix[new_point->p.x][new_point->p.y] = true;
                            new_point->last_p = temp_point;
                            new_loop.push_back(new_point);
                        }
                    }
                }
            }

        }
        if (new_loop.empty()) {
            return vector<point>();
        }
        for_loop = new_loop;
        new_loop.clear();
    }
}
