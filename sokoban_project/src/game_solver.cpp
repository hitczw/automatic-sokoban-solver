#include "game_solver.h"
#include <deque>
#include <string>
#include <iostream>
#include <time.h>

using namespace constant;
using namespace std;
//后续改进:对终点顺序调整局面估值,可以通过拉箱子的方法确定最后推动的先后顺序
//密闭空间死锁状态分析
//推动箱子改为一次推到底部的模式
//双向推导

void detect_legal::get_value(const vector<vector<char>>& maze_matrix, point& person_point) {
	//广度优先法解决迷宫,返回迷宫是否有解
	//仅当终点为通道时候有解
	deque<point> for_loop;
	for_loop.push_back(person_point);
	point new_point;

	while (!for_loop.empty()) {
		auto temp_point = for_loop.back();
		for_loop.pop_back();

		for (auto& direction : four_direction) {
			new_point = direction + temp_point;
			if (is_inside(new_point)) {//如果点在边境内
				if (value[new_point.x][new_point.y] == false) {//如果这个点没有走过
					if (maze_matrix[new_point.x][new_point.y] == BLANK) {//如果这个点是通道												
						value[new_point.x][new_point.y] = true;//这个点可以到达
						for_loop.push_front(new_point);//这个点不是终点,同样记录并压栈,并开始下一次遍历
					}
				}
			}
		}
		//for_loop遍历结束
	}
}

detect_legal::detect_legal(game_node* node) {
	value = vector<vector<bool>>(m, vector<bool>(n, false));
	node->get_matrix0(matrix_with_box);
	get_value(matrix_with_box,node->person_point);
	value[node->person_point.x][node->person_point.y] = true;
}

detect_legal::detect_legal(vector<vector<char>>& matrix,point& start) {
	matrix_with_box = matrix;
	value = vector<vector<bool>>(m, vector<bool>(n, false));
	get_value(matrix_with_box, start);
	value[start.x][start.y] = true;
}

detect_legal::detect_legal() {}

bool detect_legal::can_get(point& des) {
	//初始化完成后,仅需常数时间进行判断
	//返回当前点是否能够到达
	//printf("x is %d, y is %d\n",des.x,des.y);
	return value[des.x][des.y] == true;
}

bool detect_legal::can_box_move(point& box, point& person) {
	//判断一个箱子是否能够推动
	//初始化完成后,仅需常数时间进行判断
	point new_point;
	new_point = box * 2 - person;
	return matrix_with_box[new_point.x][new_point.y] == BLANK;//仅当这个点是通道时候可以推动
}


Astar_node::Astar_node() {
	f = 0;
	g = 0;
	h = 0;
}

Astar_node::Astar_node(game_node& c):game_node(c) {
	f = 0;
	g = 0;
	h = 0;
}

void Astar_node::get_f(vector<vector<int>>& vec) {
	//更新f值
	f = 0;
	for (auto i = box_list.begin(); i != box_list.end(); i++) {
		auto p = *i;
		f += vec[p.x][p.y];
	}
}

bool Astar_node::operator<(const Astar_node& b)const{
	return f < b.f;
}



game_solver::game_solver(string& game_map, unsigned int mm, unsigned int nn) {

	m = mm;
	n = nn;
	matrix0  =	   vector<vector<bool>>(mm, vector<bool>(nn, false));
	end_vec = vector<vector<bool>>(mm, vector<bool>(nn, false));
	blank_matrix = vector<vector<char>>(mm, vector<char>(nn, 0));

	point person_start;
	set<point> box_point_start;

	char temp_c;
	point temp_p;

	bool up = true;
	bool down = true;

	bool has_blank = false;
	bool has_end = false;

	for (char x = 0; x < m; x++) {
		for (char y = 0; y < n; y++) {
			temp_c = game_map[x*n + y];
			temp_p = { x,y };
			switch (temp_c) {
			case '#':
				blank_matrix[x][y] = WALL;
				break;
			case ' ':
				blank_matrix[x][y] = BLANK;
				break;
			case '$':
				blank_matrix[x][y] = BLANK;
				box_point_start.insert(temp_p);
				break;
			case '*':
				blank_matrix[x][y] = BLANK;
				box_point_start.insert(temp_p);
				end_vec[x][y] = true;
				break;
			case '.':
				blank_matrix[x][y] = BLANK;
				//end_list.insert(temp_p);
				end_vec[x][y] = true;
				break;
			case '@':
				blank_matrix[x][y] = BLANK;
				person_start = temp_p;
				break;
			case '+':
				blank_matrix[x][y] = BLANK;
				person_start = temp_p;
				//end_list.insert(temp_p);
				end_vec[x][y] = true;
				break;
			default:
				break;
			}
		}
	}

	init = game_node(box_point_start,person_start,0);
}

int game_solver::get_nums(game_node input) {
	//返回当前点的箱子到终点所需推动的次数
	auto p = *(input.box_list.begin());
	if (end_vec[p.x][p.y] == true) { return 0; }
	locked lc(input);
	repeat rp(input);
	int result=0;
	deque<game_node*> temp_vec;
	temp_vec.push_back(&input);
	vector<game_node*> for_del;

	while (!temp_vec.empty()) {
		game_node* node = temp_vec.back();
		temp_vec.pop_back();
		detect_legal  test(node);
		for (auto item = node->box_list.begin(); item != node->box_list.end(); item++) {
			//遍历所有箱子
			auto box = *item;//得到箱子坐标
			for (auto direction : four_direction) {
				//取4个方向
				auto new_point = box + direction;//得到箱子四周的点
				if (test.can_get(new_point)) {//如果这个点可以到达
					if (test.can_box_move(box, new_point)) {//如果在这个点可以推动箱子//存在bug,若一开始就在终点边角处

						auto new_box_point = *item * 2 - new_point;//得到新箱子的位置
						auto temp_box2 = new game_node();
						//*temp_box2 = node->get_moved(*item, new_box_point);//推动过后的节点
						node->get_moved(*item, new_box_point, temp_box2);
						vector<vector<char>>temp_matrix2;
						temp_box2->get_matrix0(temp_matrix2);
						temp_box2->last_state = node;//认父亲

						if (lc.is_locked(new_box_point, temp_matrix2) || rp.is_repeat(temp_box2)) {
							//如果这个点被锁死或者重复
							delete temp_box2;
						}

						else if (temp_box2->game_over()) {//如果发现是终点
							//cout << "here" << endl;
							while (temp_box2) {
								result += 1;
								temp_box2 = temp_box2->last_state;
							}

							for (auto &x : for_del) { delete x; }
							return result-1;
						}

						else {
							temp_vec.push_front(temp_box2);
							for_del.push_back(temp_box2);
						}
					}
				}
			}
		}
	}
	result = 1000;
	return result;
}

vector<point> game_solver::get_legal_point(vector<vector<char>>& vec, point p) {
	//返回p四周合法的独立连通域的点
	vector<point> result;
	vector<detect_legal> detect;
	bool flag = false;//如果为真代表这个点之前被检查过
	for (auto i = 0; i < 4; i++) {
		auto pp = p + four_direction[i];
		//遍历4个点
		if (vec[pp.x][pp.y] == BLANK) {
			//如果当前点是通道
			for (auto& dect : detect) {
				//遍历检查
				if (dect.can_get(pp)) {
					//如果这个点是可以到达的
					//即这个点之前已经被加入连通域
					flag = true;
					break;
				}
			}
			//遍历检查完成
			if (flag) {
				//不是新的连通域的点,继续遍历下一个点
				flag = false;
				continue;
			}
			//是新的点
			detect.push_back(detect_legal(vec,pp));//将当前点进行检查,并将检查结果压栈
			result.push_back(pp);

		}
	}
	//应该不会出现这种情况
	return result;
}

vector<vector<int>> game_solver::Astar_init() {
	//返回初始化权值矩阵
	vector<vector<int>> result(m,vector<int>(n,0));
	
	for (int i = 0; i <m; i++) {
		for (int j = 0; j < n; j++) {
			if (blank_matrix[i][j] == WALL) {
				result[i][j] = 1000;
				continue;
			}
			//这个点是空地
			game_node new_node;
			new_node.box_list.insert(point(i,j));
			vector<vector<char>>vec;
			new_node.get_matrix0(vec);
			auto person_point = get_legal_point(vec,point(i,j));
			/*cout << "箱子(" << (int)i << "," << (int)j << ")" << "附近的合法的点";
			for (auto& y : person_point) { y.show(); }*/
			if (person_point.empty()) {
				result[i][j] = 1000;
				continue;
			}
			int min_num=INT32_MAX;
			for (auto& dd : person_point){
				new_node.person_point = dd;
				int min_ = get_nums(new_node);
				if (min_ < min_num) {
					min_num = min_;
				}
			}
			result[i][j] = min_num;
		}
	}
	return result;
}

bool game_solver::cmp::operator()(const Astar_node* a, const Astar_node* b)const {
	return *a < *b;
}


vector<game_node> game_solver::Astar_solver() {
	//算法第0步,open=(s),closed=()

	auto vec = Astar_init();

	multiset<Astar_node*, cmp> open;
	repeat closed(init);
	locked lc(init);
	vector<game_node>result;
	Astar_node start_p;
	start_p = init;
	open.insert(&start_p);
	vector<Astar_node*> for_del;

	//算法第一步
	cout << "compute start!..." << endl;
	auto t1 = clock();
	while (!open.empty()) {
		//算法第二步
		auto n_min = *(open.begin());//取open列表中f值最小的
		open.erase(open.begin());//从open中移除
									//closed.is_repeat(n_min);//加入closed中

		detect_legal  test(n_min);

		for (auto item = n_min->box_list.begin(); item != n_min->box_list.end(); item++) {
			//遍历所有箱子
			auto box = *item;//得到箱子坐标
			for (auto direction : four_direction) {
				//取4个方向
				auto new_point = box + direction;//得到箱子四周的点
				if (test.can_get(new_point)) {//如果这个点可以到达
					if (test.can_box_move(box, new_point)) {//如果在这个点可以推动箱子

						auto new_box_point = *item * 2 - new_point;//得到新箱子的位置
						auto temp_box2 = new Astar_node();
						n_min->get_moved(*item, new_box_point, temp_box2);//推动过后的节点
						vector<vector<char>>temp_matrix2;
						temp_box2->get_matrix0(temp_matrix2);
						temp_box2->last_state = n_min;//认父亲

						if (lc.is_locked(new_box_point, temp_matrix2) || closed.is_repeat((temp_box2))) {
							//如果这个点被锁死或者重复
							delete temp_box2;
						}

						else if (temp_box2->game_over()) {//如果发现是终点
							cout << "compute complete!" << endl;
							auto t2 = clock();
							//(double)( t2-t1)/CLOCKS_PER_SEC
							cout << "time cost " << (double)( t2-t1)/CLOCKS_PER_SEC<< "s" << endl;
							auto nd = (game_node*)(temp_box2);

							while (nd) {
								result.push_back(*nd);
								nd = nd->last_state;
							}

							for (auto x : for_del) { delete x; }
							cout << "all status " << closed.zobrist_hash.size() << endl;
							cout << "push box times " << result.size() - 1 << endl;
							return result;
						}

						else {
							//产生新节点,且不在closed中
							temp_box2->get_f(vec);//计算f的值
							open.insert(temp_box2);//插入并重新排序
							for_del.push_back(temp_box2);
						}
					}
				}
			}
		}
		//当前节点子节点遍历完成
		//sort(open.begin(), open.end());//从新排序,影响A*算法的主要原因
	}
	cout << "unsolveble" << endl;
	exit(1000);
}       



