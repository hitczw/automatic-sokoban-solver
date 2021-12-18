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
	//������ȷ�����Թ�,�����Թ��Ƿ��н�
	//�����յ�Ϊͨ��ʱ���н�

	auto zero_matrix = matrix0;
	deque<point> for_loop;
	for_loop.push_back(person_point);

	point new_point;

	while (!for_loop.empty()) {
		auto temp_point = for_loop.back();
		for_loop.pop_back();

		for (auto& direction : four_direction) {
			new_point = direction + temp_point;
			if (is_inside(new_point)) {//������ڱ߾���
				if (zero_matrix[new_point.x][new_point.y] == false) {//��������û���߹�
					if (maze_matrix[new_point.x][new_point.y] == BLANK) {//����������ͨ��
						if (new_point == end) {//�����������յ�
							return true;
						}
						//�����յ�,�������µĵ�
						zero_matrix[new_point.x][new_point.y] = true;//��¼�߹��ĵ�
						for_loop.push_front(new_point);//����㲻���յ�,ͬ����¼��ѹջ,����ʼ��һ�α���
					}
				}
			}
		}
		
		//for_loop��������
	}
	return false;
}

bool maze_solver::dfs_maze_solver(const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
	//�������,���ݷ�

	auto zero_matrix = matrix0;
	vector<point> for_loop;

	bool push = false;

	for_loop.push_back(person_point);
	point new_point;

	while (true) {

		auto temp_point = for_loop.back();
		for (auto& direction : four_direction) {
			new_point = direction + temp_point;
			if (is_inside(new_point)) {//������ڱ߾���
				if (zero_matrix[new_point.x][new_point.y] == false) {//��������û���߹�
					if (maze_matrix[new_point.x][new_point.y] == BLANK) {//����������ͨ��
						if (new_point == end) {//�����������յ�
							//cout << "???" << endl;
							return true;
						}
						//�����յ�,�������µĵ�
						zero_matrix[new_point.x][new_point.y] = true;//��¼�߹��ĵ�
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

		if (!push) {//û�м����µĵ�
			for_loop.pop_back();
		}

		push = false;
	}
}

bool maze_solver::Astar(const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
	//A*�㷨���Թ�
	//���㷨�Դ��ͼЧ��������Ч
	//С��ͼ������

	//�㷨��0��
	multiset<Apoint> open;
	auto closed	 =matrix0;
	auto open_mat=matrix0;

	Apoint start_p;
	start_p.p = person_point;
	start_p.h = person_point.get_distance(end);
	start_p.f = start_p.h;

	open.insert(start_p);
	open_mat[start_p.p.x][start_p.p.y] = true;
	//�㷨��һ��
	while (!open.empty()) {
		//�㷨�ڶ���
		auto n_min = *(open.begin());//ȡ�ײ�Ԫ��,Ĭ�ϰ���������,��������С��
		open.erase(open.begin());//��open���Ƴ�
		open_mat[n_min.p.x][n_min.p.y] = false;//�Ƴ�
		closed[n_min.p.x][n_min.p.y] = true;//����closed
											//n_min.p.show();
		for (auto& direction : four_direction) {

			auto new_point = n_min;
			new_point.p = new_point.p + direction;
			//new_point.g = n_min.g + 1;//����gֵ,��gֵΪ0,��������㷨
			new_point.h = new_point.p.get_distance(end);
			new_point.f = new_point.g + new_point.h;//
													//���¸��ڵ���Ϣ

			if (is_inside(new_point.p)) {//������ڱ߾���
				if (closed[new_point.p.x][new_point.p.y] == false) {//��������û��closed����
					if (maze_matrix[new_point.p.x][new_point.p.y] == BLANK) {//����������ͨ��
						if (new_point.p == end) {
							return true;
						}
						//�����յ�,�������µĵ�
						if (open_mat[new_point.p.x][new_point.p.y] == false) {
							//�������open����
							open_mat[new_point.p.x][new_point.p.y] = true;
							open.insert(new_point);
						}
						//��������㷨ռʱ�������������
						//else {
						//	//��ǰ�ڵ���open����
						//	for (auto i = open.begin(); i != open.end(); i++) {
						//		if ((*i).p == new_point.p) {
						//			if (new_point.g < (*i).g) {
						//				//�µĵ�gֵ��С
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
		//��ǰ�ڵ��ӽڵ�������
		//sort(open.begin(), open.end());//��������,Ӱ��A*�㷨����Ҫԭ��
		//��set���Զ�����
	}
	return false;
}

bool maze_solver::operator()(METHOD method, const vector<vector<char>> &maze_matrix, const point &end, const point& person_point) {
	if (maze_matrix[end.x][end.y] != BLANK) {//����յ㲻��ͨ��
		return false;
	}
	else if (maze_matrix[person_point.x][person_point.y] != BLANK) {//�����㲻��ͨ��
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
	//������λ��,����д������
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
				if (is_inside(new_point->p)) {//������ڱ߾���
					if (zero_matrix[new_point->p.x][new_point->p.y] == false) {//��������û���߹�
						if (maze_matrix[new_point->p.x][new_point->p.y] == BLANK) {//����������ͨ��
							if (new_point->p == end) {//�����������յ�

								new_point->last_p = temp_point;

								vector<point> result;
								while (new_point.use_count()) {
									result.push_back(new_point->p);
									new_point = new_point->last_p;
								}
								return result;
							}
							//�����յ�,�������µĵ�
							zero_matrix[new_point->p.x][new_point->p.y] = true;//��¼�߹��ĵ�
							new_point->last_p = temp_point;
							new_loop.push_back(new_point);//����㲻���յ�,ͬ����¼��ѹջ,����ʼ��һ�α���
						}
					}
				}
			}

		}
		//for_loop��������
		if (new_loop.empty()) {
			return vector<point>();
		}
		for_loop = new_loop;
		new_loop.clear();
	}
}