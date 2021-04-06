#pragma once
//哈希表优化

#include "mazesolver.h"
#include<string>

class game_node {
	//0表示墙,1表示通道,2表示箱子
private:

public:
	 //unordered_set<point,cmp> box_list;//箱子列表
	set<point> box_list;//箱子列表
	game_node* last_state;
	point person_point;//人的位置
	//deque<game_node*> son;//当前节点子节点

	game_node(set<point>& bxp,point& ps,game_node* last=0) {
		box_list = bxp;
		person_point = ps;
		last_state = 0;
	}

	game_node(){
		last_state = 0;
	}

	void get_matrix0(vector<vector<char>>& result)const {//加上箱子
		result = blank_matrix;
		for (auto item = box_list.begin(); item != box_list.end(); item++) {
			result[(*item).x][(*item).y] = 2;
		}
	}

	vector<vector<char>> get_matrix()const {//加上箱子
		auto result = blank_matrix;

		for (auto item = box_list.begin(); item != box_list.end(); item++) {
			result[(*item).x][(*item).y] = 2;
		}
		return result;
	}

	vector<vector<char>> get_matrix2()const {
		vector<vector<char>> result;
		result = blank_matrix;

		for (auto item_box = box_list.begin();item_box != box_list.end();item_box++) {
			result[(*item_box).x][(*item_box).y] += 1;//箱子+1
		}
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (end_vec[i][j] == true) {
					result[i][j] += 2;
				}
			}
		}
		result[person_point.x][person_point.y] += 4;//人+4
		//结果为0.代表墙
		//结果为1,代表空白
		//如果点结果为2,代表是箱子
		//如果点结果为3，代表是终点
		//如果结果为4，代表箱子在终点
		//结果为5,代表人
		//结果为7，代表人在终点
		return result;
	}

	bool operator==(const game_node &a)const {
		if (a.box_list == box_list) {
			vector<vector<char>> temp_matrix2;//因为box_list相等,两个的游戏矩阵也相同
			get_matrix0(temp_matrix2);
			maze_solver maze;
			if (maze(maze_solver::dfs,temp_matrix2, a.person_point, person_point)) {
				//判断this.person_point是否能达到a.person_point的位置
				return true;
			}
		}
		return false;
	}

	inline bool game_over() {
		for (auto i = box_list.begin(); i != box_list.end(); i++) {
			auto p = *i;
			if (end_vec[p.x][p.y] == false) {return false;}
		}
		return true;
	}

	void get_moved(const point& box_before, point& box_new,game_node* result) {
		//得到新箱子位置
		//原理:将当前箱子删掉,将新的箱子加入,人是原来箱子的位置
		*result = *this;//复制当前对象
		//set<point>::iterator item;
		auto item = result->box_list.find(box_before);//找到之前的箱子
		result->box_list.erase(item);//擦掉
		result->box_list.insert(box_new);//嵌入新的箱子
		result->person_point = box_before;//人的位置是之前箱子的位置
	}
};


//结果为0.代表墙
//结果为1,代表空白
//如果点结果为2,代表是箱子
//如果点结果为3，代表是终点
//如果结果为4，代表箱子在终点
//结果为5,代表人
//结果为7，代表人在终点

