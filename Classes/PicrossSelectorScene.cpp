#include "PicrossSelectorScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* PicrossSelectorScene::createScene()
{
	auto scene = Scene::create();
	auto layer = PicrossSelectorScene::create();
	scene->addChild(layer);
	return scene;
}

bool PicrossSelectorScene::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Creating menu
	auto playItem = MenuItemImage::create("Play_Button.png",
			"Play_Button(Click).png",
			CC_CALLBACK_1(PicrossSelectorScene::goToPicrossGame, this));

	auto playItem2 = MenuItemImage::create("CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(PicrossSelectorScene::returnToMainMenu, this));

	auto menu = Menu::create(playItem, playItem2 , NULL);

	menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
	this->addChild(menu, 1);

	return true;
}

void PicrossSelectorScene::goToPicrossGame(Ref *pSender)
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	//auto scene = PicrossSelectorScene::createScene();

	//Director::getInstance()->replaceScene(TransitionFade::create(1.0,scene));
}

void PicrossSelectorScene::returnToMainMenu(Ref *pSender)
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	//auto scene = MainMenuScene::createScene();

	//Director::getInstance()->popScene();
	//Director::getInstance()->replaceScene(scene);

	auto scene = MainMenuScene::createScene();

	Director::getInstance()->replaceScene(TransitionSlideInT::create(0.5,scene));
}