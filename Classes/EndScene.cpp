#include "EndScene.h"
#include "MainMenuScene.h"
#include "PicrossSelectorScene.h"

using namespace cocos2d;
using namespace std;


USING_NS_CC;

Sprite* gameOver;



Scene* EndScene::createScene()
{
	auto scene = Scene::create();
	auto layer = EndScene::create();
	scene->addChild(layer);
	return scene;
}

bool EndScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto menuItem = MenuItemImage::create("salir2.png",
		"salir2p.png",
		CC_CALLBACK_1(EndScene::goToPicrossSelectorScene, this));
    
    auto menuItem2 = MenuItemImage::create("perder.png", "perder.png");

	auto menu = Menu::create(menuItem2, menuItem, NULL);
    
    menuItem->setScale(0.3f);
    
    menuItem2->setScale(0.18f);

    menu->alignItemsVerticallyWithPadding(visibleSize.height / 20);
    
	
    
    auto background = Sprite::create("fondo_prueba2.png");
    background->setScale(2, 2);
    background->setPosition(visibleSize.width/2, visibleSize.height /2);
    addChild(background, 0);
    addChild(menu);
    
    return true;

    
}

void EndScene::goToPicrossSelectorScene(Ref *pSender) {

	//Global::GAMEMODE = GameMode::NORMAL;

	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

    auto scene = PicrossSelectorScene::createScene();

	Director::getInstance()->replaceScene(TransitionSlideInB::create(0.5, scene));
}