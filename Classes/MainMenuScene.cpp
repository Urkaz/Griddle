#include "MainMenuScene.h"
#include "PicrossSelectorScene.h"
#include "TutorialScene.h"

USING_NS_CC;

//Constante redefinida
GameMode Constant::GAMEMODE;

Scene* MainMenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Creating menu
	auto playItem = MenuItemImage::create("boton_normal.png",
			"boton_normal_pulsado.png",
			CC_CALLBACK_1(MainMenuScene::goToNormalSelector, this));

	auto playItem2 = MenuItemImage::create("boton_libre.png",
			"boton_libre_pulsado.png",
			CC_CALLBACK_1(MainMenuScene::goToFreeSelector, this));
    
    auto playItem3 = MenuItemImage::create("boton_pausa.png",
                                           "boton_pausa.png",
                                           CC_CALLBACK_1(MainMenuScene::goToTutorialScene, this));

	auto menu = Menu::create(playItem, playItem2, NULL);
    auto menu2 = Menu::create(playItem3, NULL);

	menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
	this->addChild(menu, 1);
    this->addChild(menu2,1);

	// Adding background
	auto background = Sprite::create("Background.png");

	background->setPosition(Point((visibleSize.width  /2),
			(visibleSize.height /2)));

	addChild(background, 0);

	// Sounds
	/*CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/ButtonClick.wav");
    if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/Music.mp3");
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/Music.mp3", true);
    }*/

	return true;
}

void MainMenuScene::goToNormalSelector(Ref *pSender) {

	Constant::GAMEMODE = GameMode::NORMAL;

	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	auto scene = PicrossSelectorScene::createScene();

	Director::getInstance()->replaceScene(TransitionSlideInB::create(0.5,scene));
}

void MainMenuScene::goToFreeSelector(Ref *pSender) {

	Constant::GAMEMODE = GameMode::FREE;

	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	auto scene = PicrossSelectorScene::createScene();

	Director::getInstance()->replaceScene(TransitionSlideInB::create(0.5,scene));
}

void MainMenuScene::goToTutorialScene(Ref *pSender) {
    
    Constant::GAMEMODE = GameMode::FREE;
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");
    
    auto scene = TutorialScene::createScene();
    
    Director::getInstance()->pushScene(TransitionSlideInB::create(0.5,scene));
}
