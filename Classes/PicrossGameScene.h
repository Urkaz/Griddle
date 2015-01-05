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
	void onMouseDown(cocos2d::Event* event);
	void onMouseMove(cocos2d::Event* event);
	void onMouseUp(cocos2d::Event* event);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void goToPauseScene(Ref *pSender);
	void goToEndScene(Ref *pSender);

	void update(float dt);

private:
	//Variables
	Layer* picrossGridLayer;
	Layer* numbersLayer;
	Picross* picross;
	int lifes;
	float tiempo_ms;
	bool drawEnabled;
	bool markXEnabled;
	std::vector<std::vector<cocos2d::Sprite*>> picrossGridVector;
	std::vector<std::vector<int>> userSolution;

	std::vector<std::vector<cocos2d::Label*>> rowNumbers;
	std::vector<std::vector<cocos2d::Label*>> columnNumbers;

	//Funciones
	std::vector<std::vector<cocos2d::Sprite*>> createSquareMatrix(Picross* picross);

	cocos2d::Layer* createGridLayer(std::vector<std::vector<cocos2d::Sprite*>> vector);

	void createDecoration(std::vector<std::vector<cocos2d::Sprite*>> picrossGridVector, cocos2d::Layer* picrossLayer, Picross* picross);

	std::vector<std::vector<cocos2d::Label*>> generateNumbers(Picross* picross, bool columnsEnabled);
	cocos2d::Layer* createSquareNumbersLayer(std::vector<std::vector<cocos2d::Label*>> rows, std::vector<std::vector<cocos2d::Label*>> columns);

	void pixelLocationSquareGrid();
	void pixelLocationTriangleGrid();
};