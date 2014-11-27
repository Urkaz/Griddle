#pragma once

#include "cocos2d.h"
#include <vector>
#include <string>
#include <iostream>



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
    std::vector<std::vector<int>> matrixSolution;
	short rows;
	short columns;
	std::string name;
	std::string author;
};