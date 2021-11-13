#pragma once

#include "point.h"
#include <vector>

namespace constant {
	extern char m;
	extern char n;
	extern const point four_direction[4];
	extern std::vector<std::vector<char>> blank_matrix;
	extern std::vector<std::vector<bool>> end_vec;
	extern std::vector<std::vector<bool>> matrix0;

	bool is_inside(const point &p);
}

