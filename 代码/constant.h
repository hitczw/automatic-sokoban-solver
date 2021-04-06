#pragma once
#include"point.h"
#include <vector>
#include <set>
//�洢��Ϸ�������в��ᷢ���ı�ĳ���
//������2
//�յ���1
//ǽ��0
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

