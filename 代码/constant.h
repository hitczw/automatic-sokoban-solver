#pragma once
#include"point.h"
#include <vector>
#include <set>
//存储游戏求解过程中不会发生改变的常数
//箱子是2
//空地是1
//墙是0
namespace constant {

	char m;
	char n;
	point four_direction[4] = { {0,1},{1,0},{0,-1},{-1,0} };
	vector<vector<char>> blank_matrix;
	vector<vector<bool>> end_vec;
	vector<vector<bool>> matrix0;

	inline bool is_inside(point &p) {
		return (p.x >= 0 && p.x <= m - 1 && p.y >= 0 && p.y <= n - 1);
	}
}

