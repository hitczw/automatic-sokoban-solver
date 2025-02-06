#include "repeat.h"
#include "constant.h"
#include <cstdlib>

using namespace constant;
using namespace std;

size_t repeat::get_num::operator()(const game_node* mmm) const {
    size_t result = 0;
    for (auto x = mmm->box_list.begin(); x != mmm->box_list.end(); x++) {
        result = result ^ zobrist[(*x).x][(*x).y];
    }
    return result;
}

bool repeat::cmp::operator()(const game_node* mmm,const game_node* nnn) const {
    return *mmm == *nnn;
}

repeat::repeat(){}

void repeat::init(game_node& init) {
    zobrist = vector<vector<size_t>>(m, vector<size_t>(n, 0));
    for (auto& a : zobrist) {
        for (auto& b : a) {
            b = rand() ^ ((size_t)rand() << 15) ^ ((size_t)rand() << 30) ^ ((size_t)rand() << 45) ^ ((size_t)rand() << 60);
        }
    }
    zobrist_hash.clear();
    zobrist_hash.insert(&init);
}

bool repeat::is_repeat(const game_node* temp_box2) {
    return !zobrist_hash.insert(temp_box2).second;
}

bool repeat::is_repeat2(const game_node* temp_box2){
    return zobrist_hash.find(temp_box2) != zobrist_hash.end();
}

void repeat::insert(const game_node* temp_box2) {
    zobrist_hash.insert(temp_box2);
}

vector<vector<size_t>> repeat::zobrist;