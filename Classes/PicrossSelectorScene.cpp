#include "PicrossSelectorScene.h"
#include "MainMenuScene.h"
#include "PicrossGameScene.h"

USING_NS_CC;

short Constant::PUZZLE_NUMBER;
const int mainScale = 5;

int mainIndex = 2;

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

	//Posición inicial
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//Main
	mainPanel = new PanelSelector(1);
	mainLayer = mainPanel->getLayer();

	int layerOffsetX = visibleSize.width / 2 + mainLayer->getBoundingBox().size.width / 2;
	int layerOffsetY = visibleSize.height / 2 + mainLayer->getBoundingBox().size.height / 2;

	mainLayer->setPosition(layerOffsetX, layerOffsetY);
	mainLayer->setScale(mainScale);

	//Derecho
	if (mainIndex + 1 != Constant::MAX_PACK_INDEX)
	{
		log("RIGHT");
		rightPanel = new PanelSelector(1);
		rightLayer = rightPanel->getLayer();

		rightLayer->setPosition(visibleSize.width, mainLayer->getPositionY());
		rightLayer->setScale(mainScale / 1.7);

		addChild(rightLayer);
	}
	//Izquierdo
	if (mainIndex - 1 != 0)
	{
		log("LEFT");
		leftPanel = new PanelSelector(1);
		leftLayer = leftPanel->getLayer();

		leftLayer->setPosition(0,mainLayer->getPositionY());
		leftLayer->setScale(mainScale / 1.7);

		addChild(leftLayer);
	}

	addChild(mainLayer);

	return true;
}

void PicrossSelectorScene::onMouseDown(Event* event)
{
	auto* e = (EventMouse*)event;
	float cursorY = e->getCursorY();
	float cursorX = e->getCursorX();

	//RIGHT PANEL
	if (mainIndex + 1 != Constant::MAX_PACK_INDEX)
	{
		//Offset del rightPanel respecto al 0,0
		int rightSide = (int)(Constant::SELECTOR_SQUARE_SIDE * rightLayer->getScale());
		int rightOffSetX = rightLayer->getPosition().x - 5 / 2 * rightSide - 5 % 2 * rightSide / 2 - rightLayer->getBoundingBox().size.height / 2;
		int rightOffSetY = rightLayer->getPosition().y - 5 / 2 * rightSide - 5 % 2 * rightSide / 2 - rightLayer->getBoundingBox().size.width / 2;

		if (cursorY > rightOffSetY && cursorX > rightOffSetX &&
			cursorY < rightOffSetY + rightSide * 5 &&
			cursorX < rightOffSetX + rightSide * 5)
		{
			log("RIGHT PANEL");
		}
	}
	//LEFT PANEL
	if (mainIndex - 1 != 0)
	{
		//Offset del leftPanel respecto al 0,0
		int leftSide = (int)(Constant::SELECTOR_SQUARE_SIDE * leftLayer->getScale());
		int leftOffSetX = leftLayer->getPosition().x - 5 / 2 * leftSide - 5 % 2 * leftSide / 2 - leftLayer->getBoundingBox().size.height / 2;
		int leftOffSetY = leftLayer->getPosition().y - 5 / 2 * leftSide - 5 % 2 * leftSide / 2 - leftLayer->getBoundingBox().size.width / 2;
	
		if (cursorY > leftOffSetY && cursorX > leftOffSetX &&
			cursorY < leftOffSetY + leftSide * 5 &&
			cursorX < leftOffSetX + leftSide * 5)
		{
			log("LEFT PANEL");
		}
	}
	
	//Offset del mainPanel respecto al 0,0
	int mainSide = (int)(Constant::SELECTOR_SQUARE_SIDE * mainLayer->getScale());
	int mainOffSetX = mainLayer->getPosition().x - 5 / 2 * mainSide - 5 % 2 * mainSide / 2 - mainLayer->getBoundingBox().size.height / 2;
	int mainOffSetY = mainLayer->getPosition().y - 5 / 2 * mainSide - 5 % 2 * mainSide / 2 - mainLayer->getBoundingBox().size.width / 2;

	//MAIN PANEL
	if (cursorY > mainOffSetY && cursorX > mainOffSetX &&
		cursorY < mainOffSetY + mainSide * 5 &&
		cursorX < mainOffSetX + mainSide * 5)
	{
		//SI ESTÁ SELECCIONADO

		//Coordenadas fila(i),columna(j)
		short i = (cursorY - mainOffSetY) / mainSide;
		short j = (cursorX - mainOffSetX) / mainSide;

		i = std::abs(i - 5 + 1);

		//log("ROW:(i): %d , COL(j) %d",i,j);

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