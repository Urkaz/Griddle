#include "PicrossSelectorScene.h"
#include "MainMenuScene.h"
#include "PicrossGameScene.h"

USING_NS_CC;

short Constant::PUZZLE_NUMBER;
const int mainScale = 5;

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

	//Crear listener del ratón
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(PicrossSelectorScene::onMouseDown, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

	//Paneles
	//rightPanel = new PanelSelector(1);
	mainPanel = new PanelSelector(1);
	mainLayer = mainPanel->getLayer();
	//leftPanel = new PanelSelector(1);

	//Recalcular constante del ancho del lado de cada cuadrado del selector
	Constant::SELECTOR_SQUARE_SIDE *= mainScale;

	//Posición inicial (main por ahora solo)
	Size visibleSize = Director::getInstance()->getVisibleSize();

	int layerOffsetX = visibleSize.width / 2 + mainLayer->getBoundingBox().size.width / 2;
	int layerOffsetY = visibleSize.height / 2 + mainLayer->getBoundingBox().size.height / 2;

	mainLayer->setPosition(layerOffsetX, layerOffsetY);
	mainLayer->setScale(mainScale);

	addChild(mainLayer);

	// Creating menu
	/*auto playItem = MenuItemImage::create("Play_Button.png",
			"Play_Button(Click).png",
			CC_CALLBACK_1(PicrossSelectorScene::goToPicrossGame, this));

	auto playItem2 = MenuItemImage::create("CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(PicrossSelectorScene::returnToMainMenu, this));

	auto menu = Menu::create(playItem, playItem2 , NULL);

	menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
	this->addChild(menu, 1);*/

	return true;
}

void PicrossSelectorScene::onMouseDown(Event* event)
{
	auto* e = (EventMouse*)event;
	float cursorY = e->getCursorY();
	float cursorX = e->getCursorX();

	//Offset del tablero respecto al 0,0
	int offSetX = mainLayer->getPosition().x - 5 / 2 * Constant::SELECTOR_SQUARE_SIDE - 5 % 2 * Constant::SELECTOR_SQUARE_SIDE / 2 - mainLayer->getBoundingBox().size.height / 2;
	int offSetY = mainLayer->getPosition().y - 5 / 2 * Constant::SELECTOR_SQUARE_SIDE - 5 % 2 * Constant::SELECTOR_SQUARE_SIDE / 2 - mainLayer->getBoundingBox().size.width / 2;

	//Coordenadas fila(i),columna(j)
	short i = (cursorY - offSetY) / Constant::SELECTOR_SQUARE_SIDE;
	short j = (cursorX - offSetX) / Constant::SELECTOR_SQUARE_SIDE;

	i = std::abs(i - 5 + 1);

	//log("ROW:(i): %d , COL(j) %d",i,j);

	if (cursorY > offSetY && cursorX > offSetX &&
		cursorY < offSetY + Constant::SELECTOR_SQUARE_SIDE*5 &&
		cursorX < offSetX + Constant::SELECTOR_SQUARE_SIDE*5)
	{
		int id = mainPanel->getPicrossID(i, j);

		if (id != 0)
		{
			Constant::PUZZLE_NUMBER = id;
			log("%d", Constant::PUZZLE_NUMBER);
		}
	}
}

void PicrossSelectorScene::goToPicrossGame(Ref *pSender)
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	auto scene = PicrossGameScene::createScene();

	Director::getInstance()->pushScene(TransitionFade::create(1.0, scene));
}

void PicrossSelectorScene::returnToMainMenu(Ref *pSender)
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	//auto scene = MainMenuScene::createScene();

	//Director::getInstance()->popScene();
	//Director::getInstance()->replaceScene(scene);

	auto scene = MainMenuScene::createScene();

	Director::getInstance()->replaceScene(TransitionSlideInT::create(0.5, scene));
}