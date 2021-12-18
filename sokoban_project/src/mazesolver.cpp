#include "mazesolver.h"
#include <deque>
#include <set>
using namespace constant;
using namespace std;

npoint::npoint(const point& x) {p = x;}

npoint::npoint() {}


Apoint::Apoint() {
	f = 0;
	g = 0;
	h = 0;
	father = 0;
}

bool Apoint::operator<(const Apoint& b)const {
	if (f != b.f) { return f < b.f; }
	return p<b.p;
}


bool maze_solver::bfs_maze_solver(const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
	//广度优先法解决迷宫,返回迷宫是否有解
	//仅当终点为通道时候有解

	auto zero_matrix = matrix0;
	deque<point> for_loop;
	for_loop.push_back(person_point);

	point new_point;

	while (!for_loop.empty()) {
		auto temp_point = for_loop.back();
		for_loop.pop_back();

		for (auto& direction : four_direction) {
			new_point = direction + temp_point;
			if (is_inside(new_point)) {//如果点在边境内
				if (zero_matrix[new_point.x][new_point.y] == false) {//如果这个点没有走过
					if (maze_matrix[new_point.x][new_point.y] == BLANK) {//如果这个点是通道
						if (new_point == end) {//如果这个点是终点
							return true;
						}
						//不是终点,但是是新的点
						zero_matrix[new_point.x][new_point.y] = true;//记录走过的点
						for_loop.push_front(new_point);//这个点不是终点,同样记录并压栈,并开始下一次遍历
					}
				}
			}
		}
		
		//for_loop遍历结束
	}
	return false;
}

bool maze_solver::dfs_maze_solver(const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
	//深度优先,回溯法

	auto zero_matrix = matrix0;
	vector<point> for_loop;

	bool push = false;

	for_loop.push_back(person_point);
	point new_point;

	while (true) {

		auto temp_point = for_loop.back();
		for (auto& direction : four_direction) {
			new_point = direction + temp_point;
			if (is_inside(new_point)) {//如果点在边境内
				if (zero_matrix[new_point.x][new_point.y] == false) {//如果这个点没有走过
					if (maze_matrix[new_point.x][new_point.y] == BLANK) {//如果这个点是通道
						if (new_point == end) {//如果这个点是终点
							//cout << "???" << endl;
							return true;
						}
						//不是终点,但是是新的点
						zero_matrix[new_point.x][new_point.y] = true;//记录走过的点
						for_loop.push_back(new_point);
						push = true;
						break;
					}
				}
			}
		}

		if (for_loop.empty()) {
			return false;
		}

		if (!push) {//没有加入新的点
			for_loop.pop_back();
		}

		push = false;
	}
}

bool maze_solver::Astar(const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
	//A*算法解迷宫
	//此算法对大地图效率体现有效
	//小地图不明显

	//算法第0步
	multiset<Apoint> open;
	auto closed	 =matrix0;
	auto open_mat=matrix0;

	Apoint start_p;
	start_p.p = person_point;
	start_p.h = person_point.get_distance(end);
	start_p.f = start_p.h;

	open.insert(start_p);
	open_mat[start_p.p.x][start_p.p.y] = true;
	//算法第一步
	while (!open.empty()) {
		//算法第二步
		auto n_min = *(open.begin());//取首部元素,默认按照升序排,所以是最小的
		open.erase(open.begin());//从open中移除
		open_mat[n_min.p.x][n_min.p.y] = false;//移除
		closed[n_min.p.x][n_min.p.y] = true;//加入closed
											//n_min.p.show();
		for (auto& direction : four_direction) {

			auto new_point = n_min;
			new_point.p = new_point.p + direction;
			//new_point.g = n_min.g + 1;//更新g值,令g值为0,最佳搜索算法
			new_point.h = new_point.p.get_distance(end);
			new_point.f = new_point.g + new_point.h;//
													//更新父节点信息

			if (is_inside(new_point.p)) {//如果点在边境内
				if (closed[new_point.p.x][new_point.p.y] == false) {//如果这个点没在closed表中
					if (maze_matrix[new_point.p.x][new_point.p.y] == BLANK) {//如果这个点是通道
						if (new_point.p == end) {
							return true;
						}
						//不是终点,但是是新的点
						if (open_mat[new_point.p.x][new_point.p.y] == false) {
							//如果不在open表中
							open_mat[new_point.p.x][new_point.p.y] = true;
							open.insert(new_point);
						}
						//最佳优先算法占时不考虑下列情况
						//else {
						//	//当前节点在open表中
						//	for (auto i = open.begin(); i != open.end(); i++) {
						//		if ((*i).p == new_point.p) {
						//			if (new_point.g < (*i).g) {
						//				//新的点g值更小
						//				open.erase(i);
						//				open.insert(new_point);
						//				//*i = new_point;
						//			}
						//			//else {}
						//		}
						//	}
						//}
					}
				}
			}
		}
		//当前节点子节点遍历完成
		//sort(open.begin(), open.end());//从新排序,影响A*算法的主要原因
		//用set后自动排序
	}
	return false;
}

bool maze_solver::operator()(METHOD method, const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
	if (maze_matrix[end.x][end.y] != BLANK) {//如果终点不是通道
		return false;
	}
	else if (maze_matrix[person_point.x][person_point.y] != BLANK) {//如果起点不是通道
		return false;
	}
	else if (person_point == end) {
		return true;
	}
	switch (method) {
	case bfs:
		return bfs_maze_solver(maze_matrix, end, person_point);
	case dfs:
		return dfs_maze_solver(maze_matrix, end, person_point);
	case a_star:
		return Astar(maze_matrix, end, person_point);
	default:
		break;
	}
	return false;
}

vector<point> maze_solver::get_pace(const vector<vector<char>> &maze_matrix, const point &end, const point& start) {
	//函数换位置,不能写在这里
	if (start == end) {
		vector<point> result;
		result.push_back(start);
		return result;
	}
	auto start_p = make_shared<npoint>(start);

	auto zero_matrix = matrix0;

	vector<shared_ptr<npoint>> for_loop;
	vector<shared_ptr<npoint>> new_loop;
	for_loop.push_back(start_p);


	while (true) {
		for (auto& temp_point : for_loop) {
			for (auto& direction : four_direction) {

				auto new_point = make_shared<npoint>();

				new_point->p = direction + temp_point->p;
				if (is_inside(new_point->p)) {//如果点在边境内
					if (zero_matrix[new_point->p.x][new_point->p.y] == false) {//如果这个点没有走过
						if (maze_matrix[new_point->p.x][new_point->p.y] == BLANK) {//如果这个点是通道
							if (new_point->p == end) {//如果这个点是终点

								new_point->last_p = temp_point;

								vector<point> result;
								while (new_point.use_count()) {
									result.push_back(new_point->p);
									new_point = new_point->last_p;
								}
								return result;
							}
							//不是终点,但是是新的点
							zero_matrix[new_point->p.x][new_point->p.y] = true;//记录走过的点
							new_point->last_p = temp_point;
							new_loop.push_back(new_point);//这个点不是终点,同样记录并压栈,并开始下一次遍历
						}
					}
				}
			}

		}
		//for_loop遍历结束
		if (new_loop.empty()) {
			return vector<point>();
		}
		for_loop = new_loop;
		new_loop.clear();
	}
}