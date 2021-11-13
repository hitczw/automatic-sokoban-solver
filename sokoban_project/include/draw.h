#pragma once

#include "game_node.h"
//{ "WALL","BLANK","BOX","FINAL", "REDBOX","PERSON" ,"NULL","PERSONF" };
class draw_picture {
private:
	void draw_pic(std::vector<std::vector<char>>& matrix);

	point get_end(game_node& first, game_node& second);

	void get_complete(std::vector<game_node>& input);

	const static char symbols[8];

public:

	draw_picture();
	void draw(std::vector<game_node>& ss);
};