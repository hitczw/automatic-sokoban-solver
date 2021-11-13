#pragma once

#include "repeat.h"
#include "locked.h"
#include <string>

//后续改进:对终点顺序调整局面估值,可以通过拉箱子的方法确定最后推动的先后顺序
//密闭空间死锁状态分析
//推动箱子改为一次推到底部的模式
//双向推导

class detect_legal {
private:
	void get_value(const std::vector<std::vector<char>>& maze_matrix, point& person_point);

public:
	std::vector<std::vector<bool>> value;
	std::vector<std::vector<char>> matrix_with_box;

	detect_legal(game_node* node);

	detect_legal(std::vector<std::vector<char>>& matrix,point& start);

	detect_legal();

	bool can_get(point& des);

	bool can_box_move(point& box, point& person);
};

class Astar_node:public game_node{
public:
	int f;
	int g;
	int h;

	Astar_node();

	Astar_node(game_node& c);

	void get_f(std::vector<std::vector<int>>& vec);

	bool operator<(const Astar_node& b)const;
};

class game_solver {

public:
	//locked lc;//用于判断是否锁死
	//repeat rp;//用于判断是否是重复节点
	game_node init;//最初的起点
	
	game_solver(std::string& game_map, unsigned int mm, unsigned int nn);
	
	int get_nums(game_node input);

	std::vector<point> get_legal_point(std::vector<std::vector<char>>& vec, point p);

	std::vector<std::vector<int>> Astar_init();
	
	class cmp {
	public:
		bool operator()(const Astar_node* a, const Astar_node* b)const;
	};

	std::vector<game_node> Astar_solver();    
};



