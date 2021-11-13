#include "repeat.h"
using namespace constant;
using namespace std;

size_t repeat::get_num::operator()(const game_node* mmm) const {
	//cout << "我被执行" << endl;
	size_t result = 0;
	for (auto x = mmm->box_list.begin(); x != mmm->box_list.end(); x++) {
		result = result ^ zobrist[(*x).x][(*x).y];
	}
	return result;
}

bool repeat::cmp::operator()(const game_node* mmm,const game_node* nnn) const {
	//cout << "我被执行" << endl;
	return *mmm == *nnn;
}

repeat::repeat(){}

repeat::repeat(game_node& init) {
	zobrist = vector<vector<size_t>>(m, vector<size_t>(n, 0));
	for (auto& a : zobrist) {
		for (auto& b : a) {
			b = rand() ^ ((size_t)rand() << 15) ^ ((size_t)rand() << 30) ^ ((size_t)rand() << 45) ^ ((size_t)rand() << 60);
		}
	}
	zobrist_hash.insert(&init);
}

bool repeat::is_repeat(game_node* temp_box2) {
	return !zobrist_hash.insert(temp_box2).second;
}

vector<vector<size_t>> repeat::zobrist;

