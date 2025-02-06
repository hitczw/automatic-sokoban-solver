#pragma once

#include "game_node.h"
#include <unordered_set>

class repeat {
public:
    class get_num {
    public:
        size_t operator()(const game_node* mmm) const;
    };

    class cmp {
    public:
        bool operator()(const game_node* mmm,
                        const game_node* nnn) const;
    };

    static std::vector<std::vector<size_t>> zobrist;
    std::unordered_set<const game_node*, get_num, cmp> zobrist_hash;

    repeat();
    void init(game_node& init);
    bool is_repeat(const game_node* temp_box2);
    bool is_repeat2(const game_node* temp_box2);
    void insert(const game_node* temp_box2);
};