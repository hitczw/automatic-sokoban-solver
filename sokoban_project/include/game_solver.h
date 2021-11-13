#pragma once

#include "repeat.h"
#include "locked.h"
#include <string>

//�����Ľ�:���յ�˳����������ֵ,����ͨ�������ӵķ���ȷ������ƶ����Ⱥ�˳��
//�ܱտռ�����״̬����
//�ƶ����Ӹ�Ϊһ���Ƶ��ײ���ģʽ
//˫���Ƶ�

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
	//locked lc;//�����ж��Ƿ�����
	//repeat rp;//�����ж��Ƿ����ظ��ڵ�
	game_node init;//��������
	
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



