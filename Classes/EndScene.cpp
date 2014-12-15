#include "EndScene.h"
#include "MainMenuScene.h"

using namespace cocos2d;
using namespace std;


USING_NS_CC;


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

	auto menuItem = MenuItemImage::create("boton_pausa.png",
		"boton_click_pausa.png",
		CC_CALLBACK_1(EndScene::goToMainMenu, this));

	auto menu = Menu::create(menuItem, NULL);

	menu->setPosition(visibleSize.width - menuItem->getBoundingBox().size.width / 2, visibleSize.height - menuItem->getBoundingBox().size.height / 2);

	addChild(menu);
}

void EndScene::goToMainMenu(Ref *pSender) {

	//Constant::GAMEMODE = GameMode::NORMAL;

	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	auto scene = MainMenuScene::createScene();

	Director::getInstance()->replaceScene(TransitionSlideInB::create(0.5, scene));
}