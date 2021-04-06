#pragma once

#include "repeat.h"
#include "locked.h"
#include  <time.h>
#include <Windows.h>

//�����Ľ�:���յ�˳����������ֵ,����ͨ�������ӵķ���ȷ������ƶ����Ⱥ�˳��
//�ܱտռ�����״̬����
//�ƶ����Ӹ�Ϊһ���Ƶ��ײ���ģʽ
//˫���Ƶ�

class detect_legal {
private:
	void get_value(const vector<vector<char>>& maze_matrix, point& person_point) {
		//������ȷ�����Թ�,�����Թ��Ƿ��н�
		//�����յ�Ϊͨ��ʱ���н�
		deque<point> for_loop;
		for_loop.push_back(person_point);
		point new_point;

		while (!for_loop.empty()) {
			auto temp_point = for_loop.back();
			for_loop.pop_back();

			for (auto& direction : four_direction) {
				new_point = direction + temp_point;
				if (is_inside(new_point)) {//������ڱ߾���
					if (value[new_point.x][new_point.y] == false) {//��������û���߹�
						if (maze_matrix[new_point.x][new_point.y] == 1) {//����������ͨ��												
							value[new_point.x][new_point.y] = true;//�������Ե���
							for_loop.push_front(new_point);//����㲻���յ�,ͬ����¼��ѹջ,����ʼ��һ�α���
						}
					}
				}
			}
			//for_loop��������
		}
	}

public:
	vector<vector<bool>> value;
	vector<vector<char>> matrix_with_box;

	detect_legal(game_node* node) {
		value = vector<vector<bool>>(m, vector<bool>(n, false));
		node->get_matrix0(matrix_with_box);
		get_value(matrix_with_box,node->person_point);
		value[node->person_point.x][node->person_point.y] = true;
	}

	detect_legal(vector<vector<char>>& matrix,point& start) {
		matrix_with_box = matrix;
		value = vector<vector<bool>>(m, vector<bool>(n, false));
		get_value(matrix_with_box, start);
		value[start.x][start.y] = true;
	}

	detect_legal() {}

	bool can_get(point& des) {
		//��ʼ����ɺ�,���賣��ʱ������ж�
		//���ص�ǰ���Ƿ��ܹ�����
		return value[des.x][des.y] == true;
	}

	bool can_box_move(point& box, point& person) {
		//�ж�һ�������Ƿ��ܹ��ƶ�
		//��ʼ����ɺ�,���賣��ʱ������ж�
		point new_point;
		new_point = box * 2 - person;
		return matrix_with_box[new_point.x][new_point.y] == 1;//�����������ͨ��ʱ������ƶ�
	}
};

class Astar_node:public game_node{
public:
	int f;
	int g;
	int h;

	Astar_node() {
		f = 0;
		g = 0;
		h = 0;
	}

	Astar_node(game_node& c):game_node(c) {
		f = 0;
		g = 0;
		h = 0;
	}

	void get_f(vector<vector<int>>& vec) {
		//����fֵ
		f = 0;
		for (auto i = box_list.begin(); i != box_list.end(); i++) {
			auto p = *i;
			f += vec[p.x][p.y];
		}
	}

	bool operator<(const Astar_node& b)const{
		return f < b.f;
	}
};

class game_solver {

public:
	//locked lc;//�����ж��Ƿ�����
	//repeat rp;//�����ж��Ƿ����ظ��ڵ�
	game_node init;//��������
	

	game_solver(string& game_map, unsigned int mm, unsigned int nn) {

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
					blank_matrix[x][y] = 0;
					break;
				case ' ':
					blank_matrix[x][y] = 1;
					break;
				case '$':
					blank_matrix[x][y] = 1;
					box_point_start.insert(temp_p);
					break;
				case '*':
					blank_matrix[x][y] = 1;
					box_point_start.insert(temp_p);
					end_vec[x][y] = 1;
					break;
				case '.':
					blank_matrix[x][y] = 1;
					//end_list.insert(temp_p);
					end_vec[x][y] = 1;
					break;
				case '@':
					blank_matrix[x][y] = 1;
					person_start = temp_p;
					break;
				case '+':
					blank_matrix[x][y] = 1;
					person_start = temp_p;
					//end_list.insert(temp_p);
					end_vec[x][y] = 1;
					break;
				default:
					break;
				}
			}
		}

		init = game_node(box_point_start,person_start,0);
		
	}
	
	int get_nums(game_node input) {
		//���ص�ǰ������ӵ��յ������ƶ��Ĵ���
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
				//������������
				auto box = *item;//�õ���������
				for (auto direction : four_direction) {
					//ȡ4������
					auto new_point = box + direction;//�õ��������ܵĵ�
					if (test.can_get(new_point)) {//����������Ե���
						if (test.can_box_move(box, new_point)) {//��������������ƶ�����//����bug,��һ��ʼ�����յ�߽Ǵ�

							auto new_box_point = *item * 2 - new_point;//�õ������ӵ�λ��
							auto temp_box2 = new game_node();
							//*temp_box2 = node->get_moved(*item, new_box_point);//�ƶ�����Ľڵ�
							node->get_moved(*item, new_box_point, temp_box2);
							vector<vector<char>>temp_matrix2;
							temp_box2->get_matrix0(temp_matrix2);
							temp_box2->last_state = node;//�ϸ���

							if (lc.is_locked(new_box_point, temp_matrix2) || rp.is_repeat(temp_box2)) {
								//�������㱻���������ظ�
								delete temp_box2;
							}

							else if (temp_box2->game_over()) {//����������յ�
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

	vector<point> get_legal_point(vector<vector<char>>& vec, point p) {
		//����p���ܺϷ��Ķ�����ͨ��ĵ�
		vector<point> result;
		vector<detect_legal> detect;
		bool flag = false;//���Ϊ����������֮ǰ������
		for (auto i = 0; i < 4; i++) {
			auto pp = p + four_direction[i];
			//����4����
			if (vec[pp.x][pp.y] == 1) {
				//�����ǰ����ͨ��
				for (auto& dect : detect) {
					//�������
					if (dect.can_get(pp)) {
						//���������ǿ��Ե����
						//�������֮ǰ�Ѿ���������ͨ��
						flag = true;
						break;
					}
				}
				//����������
				if (flag) {
					//�����µ���ͨ��ĵ�,����������һ����
					flag = false;
					continue;
				}
				//���µĵ�
				detect.push_back(detect_legal(vec,pp));//����ǰ����м��,���������ѹջ
				result.push_back(pp);

			}
		}
		//Ӧ�ò�������������
		return result;
	}

	vector<vector<int>> Astar_init() {
		//���س�ʼ��Ȩֵ����
		vector<vector<int>> result(m,vector<int>(n,0));
		
		for (int i = 0; i <m; i++) {
			for (int j = 0; j < n; j++) {
				if (blank_matrix[i][j] == 0) {
					result[i][j] = 1000;
					continue;
				}
				//������ǿյ�
				game_node new_node;
				new_node.box_list.insert(point(i,j));
				vector<vector<char>>vec;
				new_node.get_matrix0(vec);
				auto person_point = get_legal_point(vec,point(i,j));
				/*cout << "����(" << (int)i << "," << (int)j << ")" << "�����ĺϷ��ĵ�";
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
	class cmp {
	public:
		bool operator()(const Astar_node* a, const Astar_node* b)const {
			return *a < *b;
		}
	};

	vector<game_node> Astar_solver() {
		//�㷨��0��,open=(s),closed=()

		auto vec = Astar_init();

		multiset<Astar_node*, cmp> open;
		repeat closed(init);
		locked lc(init);
		vector<game_node>result;
		Astar_node start_p;
		start_p = init;
		open.insert(&start_p);
		vector<Astar_node*> for_del;

		//�㷨��һ��
		cout << "���㿪ʼ" << endl;
		auto t1 = clock();
		while (!open.empty()) {
			//�㷨�ڶ���
			auto n_min = *(open.begin());//ȡopen�б���fֵ��С��
			open.erase(open.begin());//��open���Ƴ�
									 //closed.is_repeat(n_min);//����closed��

			detect_legal  test(n_min);

			for (auto item = n_min->box_list.begin(); item != n_min->box_list.end(); item++) {
				//������������
				auto box = *item;//�õ���������
				for (auto direction : four_direction) {
					//ȡ4������
					auto new_point = box + direction;//�õ��������ܵĵ�
					if (test.can_get(new_point)) {//����������Ե���
						if (test.can_box_move(box, new_point)) {//��������������ƶ�����

							auto new_box_point = *item * 2 - new_point;//�õ������ӵ�λ��
							auto temp_box2 = new Astar_node();
							n_min->get_moved(*item, new_box_point, temp_box2);//�ƶ�����Ľڵ�
							vector<vector<char>>temp_matrix2;
							temp_box2->get_matrix0(temp_matrix2);
							temp_box2->last_state = n_min;//�ϸ���

							if (lc.is_locked(new_box_point, temp_matrix2) || closed.is_repeat((temp_box2))) {
								//�������㱻���������ظ�
								delete temp_box2;
							}

							else if (temp_box2->game_over()) {//����������յ�
								cout << "�������" << endl;
								auto t2 = clock();
								cout << "����ʱ��" << t2 - t1 << "ms" << endl;
								auto nd = (game_node*)(temp_box2);

								while (nd) {
									result.push_back(*nd);
									nd = nd->last_state;
								}

								for (auto x : for_del) { delete x; }
								cout << "����״̬����" << closed.zobrist_hash.size() << endl;
								cout << "�ƶ����Ӵ���:" << result.size() - 1 << endl;
								return result;
							}

							else {
								//�����½ڵ�,�Ҳ���closed��

								temp_box2->get_f(vec);//����f��ֵ
								open.insert(temp_box2);//���벢��������
								for_del.push_back(temp_box2);
							}
						}
					}
				}
			}
			//��ǰ�ڵ��ӽڵ�������
			//sort(open.begin(), open.end());//��������,Ӱ��A*�㷨����Ҫԭ��
		}
		cout << "��ͼ�������Ϸ�޽�" << endl;
		exit(1000);
	}       
};



