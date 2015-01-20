#include "WinScene.h"
#include "PicrossSelectorScene.h"
#include "PicrossGameScene.h"
#include "Global.h"

using namespace cocos2d;
using namespace std;


USING_NS_CC;

TTFConfig LifesLabelConfig;
TTFConfig labelConfiguracion;
Label* labelGanador;
Label* LifeLabel;
Label* TimeLabel;
Texture2D::TexParams txParams;

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

<<<<<<< Updated upstream
	txParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

=======
<<<<<<< HEAD
    Texture2D::TexParams textpar = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

    
=======
	txParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

>>>>>>> FETCH_HEAD
>>>>>>> Stashed changes
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	Life = Global::LIFE;
	Time = Global::TIME;

	//Se muestran las vidas
	LifesLabelConfig.fontFilePath = "LondrinaSolid-Regular.otf";
	LifesLabelConfig.fontSize = 25;

	if (Global::GAMEMODE == GameMode::NORMAL)
	{
		LifeLabel = Label::createWithTTF(LifesLabelConfig, "Vidas " + to_string(Life));
		LifeLabel->setPosition(visibleSize.width - 150, visibleSize.height - 30);
		LifeLabel->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
		this->addChild(LifeLabel, 1);
	}

	TimeLabel = Label::createWithTTF(LifesLabelConfig, "Tiempo 0:00");
	TimeLabel->setString("Tiempo " + to_string((int)(Time / 60)) + ":" +
		(to_string((int)Time % 60).length() < 2 ? "0" + to_string((int)Time % 60) : to_string((int)Time % 60)));
	TimeLabel->setPosition(visibleSize.width - 350, visibleSize.height - 30);
	TimeLabel->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	this->addChild(TimeLabel, 1);
	
	auto menuItem = MenuItemImage::create("salirs.png",
		"salir.png",
		CC_CALLBACK_1(WinScene::goToSelectorScene, this));
    
    menuItem->setScale(0.2f);
    
    auto menuItem2 = MenuItemImage::create("superado.png", "superado.png");
    
    menuItem2->setScale(0.2f);

	auto menu = Menu::create(menuItem2, menuItem, NULL);
    
    menu->alignItemsVerticallyWithPadding(visibleSize.height/3);

	//menu->setPosition(visibleSize.width - menuItem->getBoundingBox().size.width, visibleSize.height - menuItem->getBoundingBox().size.height);
    
    
    auto background = Sprite::create("fondo_prueba2.png");
    background->setScale(2, 2);
    background->setPosition(visibleSize.width/2, visibleSize.height /2);
    addChild(background, 0);
	addChild(menu);
    
	Sprite* Sprite = Sprite::create("empty_selector.png");
	Texture2D* texture;
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
	texture = Director::getInstance()->getTextureCache()->addImage("n_" + to_string(Global::PUZZLE_NUMBER) + ".png");
    texture->setTexParameters(textpar);
=======
>>>>>>> Stashed changes
	if (Global::TIME < Global::TIME_LIMIT * 60)
		texture = Director::getInstance()->getTextureCache()->addImage("n_" + to_string(Global::PUZZLE_NUMBER) + ".png");
	else
		texture = Director::getInstance()->getTextureCache()->addImage("n_" + to_string(Global::PUZZLE_NUMBER) + "_u.png");
	
	texture->setTexParameters(txParams);

<<<<<<< Updated upstream
=======
>>>>>>> FETCH_HEAD
>>>>>>> Stashed changes
	Sprite->setTexture(texture);
	Sprite->setScale(16);
	Sprite->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 35);
	addChild(Sprite);


   	return true;
}

void WinScene::goToSelectorScene(Ref *pSender) {

	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	auto scene = PicrossSelectorScene::createScene();

	Director::getInstance()->replaceScene(TransitionSlideInB::create(0.5, scene));
}
