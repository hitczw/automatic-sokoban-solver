
#include "constant.h"
using namespace std;

namespace constant {
	char m;
	char n;
	const point four_direction[4] = { {0,1},{1,0},{0,-1},{-1,0} };
	vector<vector<char>> blank_matrix;
	vector<vector<bool>> end_vec;
	vector<vector<bool>> matrix0;

}
bool constant::is_inside(const point &p){
	return (p.x >= 0 && p.x <= m - 1 && p.y >= 0 && p.y <= n - 1);
}
