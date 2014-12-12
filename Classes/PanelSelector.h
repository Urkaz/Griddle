#pragma once

#include "cocos2d.h"
#include <vector>

class PanelSelector
{
public:
	PanelSelector(int num);

	cocos2d::Layer* getLayer();
	int getPicrossID(int row, int col);

private:
	cocos2d::Layer* gridLayer;

	std::vector<std::vector<cocos2d::Sprite*>> gridVector;
	std::vector<std::vector<int>> picrossID;
	std::string name;
	int packID;

	void readPanel(int num);
	void createLayer();
};

