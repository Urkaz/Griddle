#include "WinScene.h"
#include "PicrossSelectorScene.h"

using namespace cocos2d;
using namespace std;


USING_NS_CC;


TTFConfig labelConfiguracion;
Label* labelGanador;


Scene* WinScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WinScene::create();
	scene->addChild(layer);
	return scene;
}

bool WinScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menuItem = MenuItemImage::create("salirs.png",
		"salir.png",
		CC_CALLBACK_1(WinScene::goToSelectorScene, this));
    
    menuItem->setScale(0.2);
    
    auto menuItem2 = MenuItemImage::create("superado.png", "superado.png");
    
    menuItem2->setScale(0.2);

	auto menu = Menu::create(menuItem2, menuItem, NULL);
    
    menu->alignItemsVerticallyWithPadding(visibleSize.height/10);

	//menu->setPosition(visibleSize.width - menuItem->getBoundingBox().size.width, visibleSize.height - menuItem->getBoundingBox().size.height);
    
    
    auto background = Sprite::create("fondo_prueba2.png");
    background->setScale(2, 2);
    background->setPosition(visibleSize.width/2, visibleSize.height /2);
    addChild(background, 0);
	addChild(menu);
    
    
    

	return true;


}

void WinScene::goToSelectorScene(Ref *pSender) {

	//Constant::GAMEMODE = GameMode::NORMAL;

	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	auto scene = PicrossSelectorScene::createScene();

	Director::getInstance()->replaceScene(TransitionSlideInB::create(0.5, scene));
}