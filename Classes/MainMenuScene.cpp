#include "MainMenuScene.h"
#include "PicrossSelectorScene.h"
#include "TutorialScene.h"

using namespace cocos2d;
using namespace std;

//Constante redefinida
GameMode Global::GAMEMODE;
bool Global::DEBUG;

Texture2D::TexParams params;
Texture2D* titulotexture;
Texture2D* aviontexture;

MoveBy* movebyy;
MoveBy* move_back;
EaseBackInOut* move_easy_in;

DelayTime* delay;
Sequence* seq1;

bool tuturialactivo = false;

RotateTo* rotacion;
RotateTo* rotacionabajo;

FadeIn* fIn;

Sprite* titulo;
Sprite* avion;


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

	Global::DEBUG = false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Texture2D::TexParams params = {GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    

	// Creating menu
    if (tuturialactivo)
    {
		auto normal = MenuItemImage::create("modo_normal.png",
			"modo_normal_p.png",
			CC_CALLBACK_1(MainMenuScene::goToTutorialScene, this));

		auto libre = MenuItemImage::create("modo_libre.png",
			"modo_libre_p.png",
			CC_CALLBACK_1(MainMenuScene::goToTutorialScene, this));

		auto bomba = MenuItemImage::create("modo_libre.png",
			"modo_libre_p.png",
			CC_CALLBACK_1(MainMenuScene::goToTutorialScene, this));
        
		auto menu = Menu::create(normal, libre, bomba, NULL);
        
        menu->alignItemsHorizontallyWithPadding(visibleSize.height / 7);
        this->addChild(menu, 1);
    }
    else
    {
		auto normal = MenuItemImage::create("modo_normal.png",
			"modo_normal_p.png",
			CC_CALLBACK_1(MainMenuScene::goToNormalSelector, this));

		auto libre = MenuItemImage::create("modo_libre.png",
			"modo_libre_p.png",
			CC_CALLBACK_1(MainMenuScene::goToFreeSelector, this));

		auto bomba = MenuItemImage::create("modo_bomba.png",
			"modo_bomba_p.png",
			CC_CALLBACK_1(MainMenuScene::goToBombSelector, this));

		auto tutorial = MenuItemImage::create("boton_nube_tutorial.png",
             "boton_tutorial_p.png",
			 CC_CALLBACK_1(MainMenuScene::goToTutorialScene, this));

		auto borrar = MenuItemImage::create("boton_borrar.png",
			"boton_borrar_p.png",
			CC_CALLBACK_1(MainMenuScene::deleteSaveData, this));

		auto menu = Menu::create(normal, libre, bomba, NULL);

		menu->alignItemsHorizontallyWithPadding(visibleSize.height / 7);
		this->addChild(menu, 1);

		auto menu2 = Menu::create(tutorial, borrar, NULL);

		menu2->alignItemsHorizontallyWithPadding(visibleSize.height / 20);
		menu2->setPositionY(menu->getPositionY() -100);
		this->addChild(menu2, 1);
    }

	// Adding background
	auto background = Sprite::create("fondonubes.png");

	background->setPosition(Point((visibleSize.width  /2),
			(visibleSize.height /2)));

	addChild(background, 0);
    
    titulo = Sprite::create("tituloverde.png");
    titulotexture = Director::getInstance()->getTextureCache()->addImage("tituloverde.png");
    titulotexture->setTexParameters(params);
    titulo->setTexture(titulotexture);
    titulo->setPosition(Point((visibleSize.width/2), (visibleSize.height/1.3)));
    titulo->setScale(0.9f);
    fIn = FadeIn::create(1.0f);
    titulo->runAction(fIn);
    addChild(titulo);
    
    avion = Sprite::create("avion.png");
    aviontexture = Director::getInstance()->getTextureCache()->addImage("avion.png");
    aviontexture->setTexParameters(params);
    avion->setTexture(aviontexture);
    avion->setPosition(Point((visibleSize.width/9), (visibleSize.height/9)));
    avion->setScale(4);
    addChild(avion);

    rotacion = RotateTo::create(0.3f, -20.0f);
    rotacionabajo = RotateTo::create(0.3f, -190.0f);
    
    movebyy = MoveBy::create(2, Vec2(visibleSize.width, visibleSize.height/3));
    move_back = movebyy->reverse();
    move_easy_in = EaseBackInOut::create(movebyy->clone());
    delay = DelayTime::create(2.0f);
    seq1 = Sequence::create(rotacion, move_easy_in, rotacionabajo, delay, move_back, delay->clone(),rotacion, nullptr);
    avion->runAction(RepeatForever::create(seq1));
    
	if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("cancionmenu.wav");
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("cancionmenu.wav", true);
	}


	// Sounds
	/*CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/ButtonClick.wav");
    if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/Music.mp3");
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/Music.mp3", true);
    }*/

	return true;
}

void MainMenuScene::goToNormalSelector(Ref *pSender) {

	Global::GAMEMODE = GameMode::NORMAL;

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("cambio_escena.wav");

	auto scene = PicrossSelectorScene::createScene();

	Director::getInstance()->replaceScene(TransitionFadeTR::create(1,scene));
    
}

void MainMenuScene::goToFreeSelector(Ref *pSender) {

	Global::GAMEMODE = GameMode::FREE;

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("cambio_escena.wav");

	auto scene = PicrossSelectorScene::createScene();

	Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
}

void MainMenuScene::goToBombSelector(Ref *pSender) {

	Global::GAMEMODE = GameMode::BOMB;

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("cambio_escena.wav");

	auto scene = PicrossSelectorScene::createScene();

	Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
}

void MainMenuScene::goToTutorialScene(Ref *pSender) {
    
    //Global::GAMEMODE = GameMode::FREE;
    
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("cambio_escena.wav");
    
    auto scene = TutorialScene::createScene();
    
    Director::getInstance()->pushScene(TransitionFadeTR::create(1, scene));

}

void MainMenuScene::deleteSaveData(Ref *pSender)
{
	//Borrar datos picross
	for (int i = 0; i < 24; i++)
	{
		UserDefault::getInstance()->setBoolForKey(("n_" + to_string(i)).c_str(), false);
		UserDefault::getInstance()->setIntegerForKey(("n_" + to_string(i) + "_fallos").c_str(), -1);
		UserDefault::getInstance()->setIntegerForKey(("n_" + to_string(i) + "_tiempo").c_str(), 0);
	}

	UserDefault::getInstance()->flush();
}
