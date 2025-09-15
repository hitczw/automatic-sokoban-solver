#include "constant.h"
using namespace std;

int8_t constant::m;
int8_t constant::n;
const point constant::four_direction[4] = { {0, 1}, {1, 0},{0, -1},{-1, 0} };
my_memory_pool constant::maze_mp;
vector<vector<char>> constant::blank_matrix;
vector<vector<bool>> constant::end_vec;
vector<vector<bool>> constant::matrix0;

bool constant::is_inside(const point &p){
    return p.x >= 0 && p.x <= m - 1 && p.y >= 0 && p.y <= n - 1;
}
