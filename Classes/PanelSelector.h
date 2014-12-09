#pragma once

#include "cocos2d.h"
#include <vector>

class PanelSelector
{
public:
	PanelSelector(int num);

private:

	void readPanel(int num);
	void createGrid();

	std::vector<std::vector<cocos2d::Sprite*>> gridVector;
	std::vector<std::vector<int>> picrossID;
	std::string name;
};

