#pragma once

#include "game_node.h"
#include <unordered_set>
#include <cstddef>
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
    std::unordered_set<game_node*, get_num,cmp> zobrist_hash;

    repeat();
    repeat(game_node& init);

    bool is_repeat(game_node* temp_box2);
};