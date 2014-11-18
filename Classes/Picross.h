#pragma once

#include "cocos2d.h"
#include <vector>

enum class GameMode {NORMAL, FREE, BOMB, TRIANGLES};

class Picross
{
public:
	Picross(int num, GameMode gm);

	short getRowNumber();
	short getColumnNumber();

private:

	std::vector<int> solution;
	short rows;
	short columns;
};