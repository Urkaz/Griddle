#include "MainMenuScene.h"
//#include "GameScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Creating menu
	auto playItem = MenuItemImage::create("Play_Button.png",
			"Play_Button(Click).png",
			CC_CALLBACK_1(MainMenuScene::goToGameScene, this));

	auto menu = Menu::create(playItem, NULL , NULL);

	menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
	this->addChild(menu, 1);

	// Adding background

	auto background = Sprite::create("Background.png");

	background->setPosition(Point((visibleSize.width  /2),
			(visibleSize.height /2)));

	addChild(background, 0);



	/*auto menuTitle = MenuItemImage::create("MainMenuScreen/Game_Title.png", "MainMenuScreen/Game_Title.png");

    auto playItem = MenuItemImage::create("MainMenuScreen/Play_Button.png",
                                          "MainMenuScreen/Play_Button(Click).png",
                                          CC_CALLBACK_1(MainMenuScene::goToGameScene, this));

    auto menu = Menu::create(menuTitle, playItem, NULL);

    menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
    this->addChild(menu, 1);

    // Adding background
    auto background = Sprite::create("MainMenuScreen/Background.png");

    background->setPosition(Point((visibleSize.width  /2),
                                  (visibleSize.height /2)));

    addChild(background, 0);*/

	//Preloading sounds
	/*CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/ButtonClick.wav");
    if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/Music.mp3");
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/Music.mp3", true);
    }*/

	return true;
}

void MainMenuScene::goToGameScene(Ref *pSender) {
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	//auto scene = GameScene::createScene();

	//Director::getInstance()->replaceScene(TransitionFade::create(1.0,scene));
}
