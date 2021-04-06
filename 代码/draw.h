#pragma once
#include<graphics.h>
#include<conio.h>
#include "game_node.h"

class draw_picture {
private:
	LPCTSTR get_picture[8] = { "WALL","BLANK","BOX","FINAL", "REDBOX","PERSON" ,"NULL","PERSONF" };
	IMAGE	get_pic[8];

	void draw_pic(vector<vector<char>>& matrix) {

		for (int a = 0; a < m; a++) {
			for (int b = 0; b < n; b++) {
				auto x = get_pic[matrix[a][b]];
				putimage(b * 50, a * 50, &x);
			}
		}
	}

	point get_end(game_node& first, game_node& second) {
		auto s_box = second.box_list;
		auto f_box = first.box_list;
		point p;
		for (auto s = s_box.begin(); s != s_box.end(); s++) {
			if (f_box.find(*s) == f_box.end()) {//这个点在second中有,first中无
				p = *s;
				break;
			}
		}
		return second.person_point * 2 - p;
	}

	void get_complete(vector<game_node>& input) {
		vector<game_node> output;
		output.push_back(input[0]);
		maze_solver maze;
		for (int i = 0; i < input.size() - 1; i++) {
			auto first = input[i + 1];
			auto second = input[i];
			auto end_p = get_end(first, second);
			auto path = maze.get_pace(first.get_matrix(), end_p, first.person_point);

			for (int j = 0; j < path.size(); j++) {
				first.person_point = path[j];
				output.push_back(first);
			}
		}

		input = output;
	}

public:
	int height;
	int wide;

	draw_picture() {
		for (int i = 0; i < 8; i++) {
			auto y = get_picture[i];
			IMAGE x;
			
			loadimage(&x, "IMAGE", y);
			get_pic[i] = x;
			wide = x.getheight();
			height = x.getwidth();
		}
	}

	void draw(vector<game_node>& ss) {

		get_complete(ss);
		initgraph(n * height, m * wide);
		int i = ss.size() - 1;
		auto matrix = ss[i].get_matrix2();
		draw_pic(matrix);
		while (1) {

			auto key = _getch();
			if (key == 'w') {
				i = (i == 0 ? 0 : (i - 1));
				matrix = ss[i].get_matrix2();
				draw_pic(matrix);
			}
			else if (key == 's') {
				i = (i == (ss.size() - 1) ? i : (i + 1));
				matrix = ss[i].get_matrix2();
				draw_pic(matrix);
			}
			else if (key == ' ') {
				i = ss.size() - 1;
				matrix = ss[i].get_matrix2();
				draw_pic(matrix);
			}
		}
	}
};