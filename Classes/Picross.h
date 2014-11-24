#pragma once

#include "cocos2d.h"
#include <vector>

enum class GameMode {NORMAL, FREE, BOMB, TRIANGLES};

class Picross
{
public:
	Picross(short num, GameMode gm);

	short getRowNumber();
	short getColumnNumber();

	std::vector<std::vector<int>> getSolution();

private:

	std::vector<std::vector<int>> solution;
	short rows;
	short columns;
};