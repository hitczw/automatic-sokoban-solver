#pragma once
#include "game_node.h"

class locked {
private:
	vector<vector<bool>> side_point;
	bool locked_double(vector<vector<char>>& matrix_with_box, point& box, point& wall) {//�����������

		auto bx = box.x;
		auto by = box.y;
		auto wx = wall.x;
		auto wy = wall.y;
		if (bx == wx) {//����ͬһ��
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
			if (is_inside(new_point)) {//������ڱ߽���
				if (blank_matrix[new_point.x][new_point.y] == 0) {//�������ǽ
					result.push_back(new_point);
				}
			}
		}
		return result;
	}

	bool is_next_two_wall(vector<point> &around) {//�ж��Ƿ�������ǽ���ڵ����
		if (around.size() <= 1) {
			return false;
		}
		if (around.size() >= 3) {
			return true;
		}
		//ʣ��������ǽ

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
				//�ų��ײ��������
				if (blank_matrix[i][j] == 0) {//�������ש�� 
					side_point[i][j] = true;//���������Ӳ����Ƶ�ש����,���ϸ����,��һ��Ҳ�Ƕ�����
					if (has_blank && up_down_lock && (!has_end)) {

						//�������֮ǰ�����հ�������������û�������յ�
						for (char k = last_j; k < j; k++) {
							side_point[i][k] = true;
							//side_point.insert(point(i, k));
						}
					}
					//���������������һ��
					//up = true;
					//down = true;
					up_down_lock = true;
					has_blank = false;
					has_end = false;
					continue;
				}

				if (blank_matrix[i][j] == 1) {//�����հ�
					if (!has_blank) {//���֮ǰû�������հ�
						last_j = j;//��¼��ʼ�����հ׵�λ��
						has_blank = true;
					}
					if (end_vec[i][j]==true) {
						//�������հ����յ�
						has_end = true;
					}
				}
				//if (blank_matrix[i - 1][j] == 1) { up = false; }
				//if (blank_matrix[i + 1][j] == 1) { down = false;}
				if(blank_matrix[i-1][j]==1&&blank_matrix[i+1][j]==1){
					//�����¶��ǿյ�ʱ��,�����Ͳ�������״̬
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
				if (blank_matrix[i][j] == 0) {//�������ש�� 
					side_point[i][j] = true; //���������Ӳ����Ƶ�ש����, ���ϸ����, ��һ��Ҳ�Ƕ�����
					if (has_blank && up_down_lock && (!has_end)) {
						//�������֮ǰ�����հ����ϻ���û�������հ���û�������յ�
						for (char k = last_i; k < i; k++) {
							side_point[k][j] = true;
							//side_point.insert(point(k, j));
						}

					}
					//���������������һ��
					//up = true;
					//down = true;
					up_down_lock = true;
					has_blank = false;
					has_end = false;

					continue;
				}

				if (blank_matrix[i][j] == 1) {//�����հ�
					if (!has_blank) {//���֮ǰû�������հ�
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
					//�����¶��ǿյ�ʱ��,�����Ͳ�������״̬
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
				if (blank_matrix[i][j] == 1) {//����������ͨ��
					auto box = point(i, j);
					auto around = get_box_wall(box);
					if (around.empty()) {//���û�б�����
						continue;//û����ǽ,��������״̬
					}

					if (is_next_two_wall(around)) {//����ǽ���ڵ����

						if (end_vec[i][j] == false) {
							//side_point.insert(box);
							side_point[box.x][box.y] = true;
						}
					}
				}
			}
		}

	}

	inline bool is_locked(point& box, vector<vector<char>>& matrix_with_box) {//�ж������Ƿ�������״̬
		//auto f = side_point.find(box);
		//if (f != side_point.end()) { return true; }//����ڱ߽�����
		if (side_point[box.x][box.y] == true) { return true; }
		if (end_vec[box.x][box.y] == true) {
			return false;
		}

		auto around = get_box_wall(box);
		if (around.empty()) { return false; }
		return locked_double(matrix_with_box, box, around[0]);
	}
};
