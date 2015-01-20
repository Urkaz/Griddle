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

	int getRowNumber();
	int getColumnNumber();

	std::string getName();
	std::string getAuthor();

	std::vector<std::vector<int>> getSolution();
	int getSolutionNum();

private:

    std::vector<std::vector<int>> matrixSolution;
	short rows;
	short columns;
	std::string name;
	std::string author;
	int solutionNum;
};