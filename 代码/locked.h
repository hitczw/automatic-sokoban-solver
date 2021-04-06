#pragma once
#include "game_node.h"

class locked {
private:
	vector<vector<bool>> side_point;
	bool locked_double(vector<vector<char>>& matrix_with_box, point& box, point& wall) {//并排锁死情况

		auto bx = box.x;
		auto by = box.y;
		auto wx = wall.x;
		auto wy = wall.y;
		if (bx == wx) {//属于同一行
			auto box_down = matrix_with_box[bx + 1][by];
			auto box_up = matrix_with_box[bx - 1][by];
			auto wall_down = matrix_with_box[wx + 1][wy];
			auto wall_up = matrix_with_box[wx - 1][wy];
			auto wall_down_op = matrix_with_box[wx + 1][2 * by - wy];
			auto wall_up_op = matrix_with_box[wx - 1][2 * by - wy];

			return (box_up == 2 && (wall_up == 0 || wall_up_op == 0 || wall_up == 2)) ||
				   (box_down == 2 && (wall_down == 0 || wall_down_op == 0 || wall_down == 2));
		}
		else if (by == wy) {
			auto box_right = matrix_with_box[bx][by + 1];
			auto box_left = matrix_with_box[bx][by - 1];
			auto wall_right = matrix_with_box[wx][wy + 1];
			auto wall_left = matrix_with_box[wx][wy - 1];
			auto wall_right_op = matrix_with_box[2 * bx - wx][wy + 1];
			auto wall_left_op = matrix_with_box[2 * bx - wx][wy - 1];

			return (box_right == 2 && (wall_right == 0 || wall_right_op == 0 || wall_right == 2)) || \
				(box_left == 2 && (wall_left == 0 || wall_left_op == 0 || wall_left == 2));
		}
		return false;
	}

	vector<point> get_box_wall(point &box) {
		vector<point> result;
		point new_point;
		for (auto direction : four_direction) {
			new_point = direction + box;
			if (is_inside(new_point)) {//如果点在边界内
				if (blank_matrix[new_point.x][new_point.y] == 0) {//如果点是墙
					result.push_back(new_point);
				}
			}
		}
		return result;
	}

	bool is_next_two_wall(vector<point> &around) {//判断是否是两面墙相邻的情况
		if (around.size() <= 1) {
			return false;
		}
		if (around.size() >= 3) {
			return true;
		}
		//剩下有两面墙

		return abs(around[0].x - around[1].x) == 1;
	}

public:
	locked(){}

	locked(game_node& init) {
		side_point = matrix0;
		side_point[0][0] = true;
		side_point[0][n - 1] = true;
		side_point[m - 1][0] = true;
		side_point[m - 1][n - 1] = true;
		//bool up = true;
		//bool down = true;
		bool up_down_lock = true;

		bool has_blank = false;
		bool has_end = false;

		char last_j = 1;
		char last_i = 1;

		for (char i = 1; i < m - 1; i++) {
			for (char j = 0; j < n; j++) {
				//排除底部锁死情况
				if (blank_matrix[i][j] == 0) {//如果遇到砖块 
					side_point[i][j] = true;//理论上箱子不会推到砖块上,但严格起见,这一点也是堵死的
					if (has_blank && up_down_lock && (!has_end)) {

						//如果遇到之前遇到空白且上下锁死且没有遇到终点
						for (char k = last_j; k < j; k++) {
							side_point[i][k] = true;
							//side_point.insert(point(i, k));
						}
					}
					//其他情况，遍历下一个
					//up = true;
					//down = true;
					up_down_lock = true;
					has_blank = false;
					has_end = false;
					continue;
				}

				if (blank_matrix[i][j] == 1) {//遇到空白
					if (!has_blank) {//如果之前没有遇到空白
						last_j = j;//记录初始遇到空白的位置
						has_blank = true;
					}
					if (end_vec[i][j]==true) {
						//如果这个空白是终点
						has_end = true;
					}
				}
				//if (blank_matrix[i - 1][j] == 1) { up = false; }
				//if (blank_matrix[i + 1][j] == 1) { down = false;}
				if(blank_matrix[i-1][j]==1&&blank_matrix[i+1][j]==1){
					//当上下都是空的时候,这个点就不是锁死状态
					up_down_lock = false;
				}
			}
			//up = true;
			//down = true;
			up_down_lock = true;
			has_blank = false;
			has_end = false;
		}

		for (char j = 1; j < n - 1; j++) {
			for (char i = 0; i < m; i++) {
				if (blank_matrix[i][j] == 0) {//如果遇到砖块 
					side_point[i][j] = true; //理论上箱子不会推到砖块上, 但严格起见, 这一点也是堵死的
					if (has_blank && up_down_lock && (!has_end)) {
						//如果遇到之前遇到空白且上或下没有遇到空白且没有遇到终点
						for (char k = last_i; k < i; k++) {
							side_point[k][j] = true;
							//side_point.insert(point(k, j));
						}

					}
					//其他情况，遍历下一个
					//up = true;
					//down = true;
					up_down_lock = true;
					has_blank = false;
					has_end = false;

					continue;
				}

				if (blank_matrix[i][j] == 1) {//遇到空白
					if (!has_blank) {//如果之前没有遇到空白
						last_i = i;
						has_blank = true;
					}

					if (end_vec[i][j] == true) {
						has_end = true;
					}

				}
				//if (blank_matrix[i][j - 1] == 1) { up = false; }
				//if (blank_matrix[i][j + 1] == 1) { down = false; }
				if (blank_matrix[i][j-1] == 1 && blank_matrix[i][j+1] == 1) {
					//当上下都是空的时候,这个点就不是锁死状态
					up_down_lock = false;
				}
			}
			//up = true;
			//down = true;
			up_down_lock = true;
			has_blank = false;
			has_end = false;
		}
		for (char i = 0; i < m; i++) {
			for (char j = 0; j < n; j++) {
				if (blank_matrix[i][j] == 1) {//如果这个点是通道
					auto box = point(i, j);
					auto around = get_box_wall(box);
					if (around.empty()) {//如果没有被锁死
						continue;//没有邻墙,不是锁死状态
					}

					if (is_next_two_wall(around)) {//两面墙相邻的情况

						if (end_vec[i][j] == false) {
							//side_point.insert(box);
							side_point[box.x][box.y] = true;
						}
					}
				}
			}
		}

	}

	inline bool is_locked(point& box, vector<vector<char>>& matrix_with_box) {//判断箱子是否是锁死状态
		//auto f = side_point.find(box);
		//if (f != side_point.end()) { return true; }//如果在边角锁死
		if (side_point[box.x][box.y] == true) { return true; }
		if (end_vec[box.x][box.y] == true) {
			return false;
		}

		auto around = get_box_wall(box);
		if (around.empty()) { return false; }
		return locked_double(matrix_with_box, box, around[0]);
	}
};
