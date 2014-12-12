#pragma once

#include "cocos2d.h"
#include "Picross.h"
#include "PanelSelector.h"

class PicrossSelectorScene: public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(PicrossSelectorScene);
    
    // Added
	void goToPicrossGame(Ref *pSender);
	void returnToMainMenu(Ref *pSender);

	void onMouseDown(cocos2d::Event* event);

private:
	PanelSelector* rightPanel;
	PanelSelector* mainPanel;
	PanelSelector* leftPanel;

	Layer* rightLayer;
	Layer* mainLayer;
	Layer* leftLayer;
};