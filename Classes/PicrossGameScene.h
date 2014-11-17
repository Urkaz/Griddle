#pragma once

#include "cocos2d.h"
#include "Picross.h"


class PicrossGameScene: public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(PicrossGameScene);
    
    // Added


	//void goToPicrossGame(Ref *pSender);
	//void returnToMainMenu(Ref *pSender);

private:
	std::vector<cocos2d::Sprite*> picrossGrid;
	Picross* picross;
	//int userSolution;

	std::vector<cocos2d::Sprite*> createSquareGrid();
	std::vector<cocos2d::Sprite*> createTriangleGrid();
};