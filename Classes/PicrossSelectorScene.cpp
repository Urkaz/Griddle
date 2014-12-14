#include "PicrossSelectorScene.h"
#include "MainMenuScene.h"
#include "PicrossGameScene.h"

USING_NS_CC;

short Constant::PUZZLE_NUMBER;
const int mainScale = 5;

int mainIndex = 1;

//Variables para la animación de scroll
bool moveLeft = false;
bool movetrue = false;
float leftCount = 0, mainCount = 0, rightCount = 0, auxCount = 0;
float mainCountScale = 0;
float sideSpace, mainSpace, mainScaleSpace, mov;
bool leftFinish = false;
bool mainFinish = false;
bool rightFinish = false;
bool scaleFinish = false;
bool auxFinish = false;
bool initAnim = true;

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
	mainPanel = new PanelSelector(mainIndex);
	mainLayer = mainPanel->getLayer();

	mainLayer->setScale(mainScale);

	int layerOffsetX = visibleSize.width / 2 + mainLayer->getBoundingBox().size.width / 2;
	int layerOffsetY = visibleSize.height / 2 + mainLayer->getBoundingBox().size.height / 2;

	mainLayer->setPosition(layerOffsetX, layerOffsetY);

	//Derecho
	if (mainIndex + 1 != Constant::MAX_PACK_INDEX)
	{
		rightPanel = new PanelSelector(mainIndex+1);
		rightLayer = rightPanel->getLayer();

		rightLayer->setScale(mainScale / 1.7);
		rightLayer->setPosition(visibleSize.width + rightLayer->getBoundingBox().size.width/2, mainLayer->getPositionY());

		addChild(rightLayer);
	}
	//Izquierdo
	if (mainIndex - 1 != 0)
	{
		leftPanel = new PanelSelector(mainIndex-1);
		leftLayer = leftPanel->getLayer();

		leftLayer->setScale(mainScale / 1.7);
		leftLayer->setPosition(0 + leftLayer->getBoundingBox().size.width / 2, mainLayer->getPositionY());

		addChild(leftLayer);
	}

	addChild(mainLayer);

	this->scheduleUpdate();

	return true;
}

void PicrossSelectorScene::onMouseDown(Event* event)
{
	auto* e = (EventMouse*)event;
	float cursorY = e->getCursorY();
	float cursorX = e->getCursorX();

	//RIGHT PANEL
	if (mainIndex + 1 <= Constant::MAX_PACK_INDEX)
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
			enableLeftAnim();
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
			//movePanelsToRight();
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

void PicrossSelectorScene::update(float dt)
{
	if (moveLeft)
	{
		movePanelsToLeft(dt);
	}
}

void PicrossSelectorScene::enableLeftAnim()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//desplazamientos totales a realizar
	sideSpace = Constant::SELECTOR_SQUARE_SIDE * mainScale / 1.7 * 5 / 2;
	mainSpace = mainLayer->getPositionX();
	mainScaleSpace = mainScale - mainScale / 1.7;

	if (mainIndex + 2 <= Constant::MAX_PACK_INDEX)
	{
		auxPanel = new PanelSelector(mainIndex + 2);
		auxLayer = auxPanel->getLayer();
		auxLayer->setPosition(visibleSize.width + sideSpace, rightLayer->getPositionY());
		auxLayer->setScale(rightLayer->getScale());
		addChild(auxLayer);
	}
	if (mainIndex + 1 <= Constant::MAX_PACK_INDEX)
	{
		moveLeft = true;
	}
}

void PicrossSelectorScene::movePanelsToLeft(float dt)
{
	mov = 300 * dt;

	//Izquiero sale de la pantalla
	if (sideSpace > leftCount + mov && mainIndex - 1 != 0)
	{
		leftLayer->setPositionX(leftLayer->getPositionX() - mov);
		leftCount += mov;
	}
	else leftFinish = true;

	//Principal se va a la izquerda
	if (mainSpace > mainCount + mov*1.3)
	{
		mainLayer->setPositionX(mainLayer->getPositionX() - mov*1.3);
		mainCount += mov * 1.3;
	}
	else mainFinish = true;

	//Derecho se va al medio
	if (mainSpace > rightCount + mov*1.3)
	{
		rightLayer->setPositionX(rightLayer->getPositionX() - mov*1.3);
		rightCount += mov*1.3;
	}
	else rightFinish = true;

	//SCALE
	if (mainScaleSpace > mainCountScale + 2 * dt)
	{
		mainLayer->setScale(mainLayer->getScale() - 2 * dt);
		rightLayer->setScale(rightLayer->getScale() + 2 * dt);

		mainCountScale += 2 * dt;
	}
	else scaleFinish = true;

	//El nuevo panel entra en pantalla
	if (scaleFinish && mainIndex + 2 <= Constant::MAX_PACK_INDEX)
	{
		if (sideSpace > auxCount + mov)
		{
			auxLayer->setPositionX(auxLayer->getPositionX() - mov);
			auxCount += mov;
		}
		else auxFinish = true;
	}

	if (leftFinish && mainFinish && rightFinish && auxFinish)
	{

		removeChild(leftLayer);

		//swap de paneles y variables
		leftPanel = mainPanel;
		leftLayer = leftPanel->getLayer();

		mainPanel = rightPanel;
		mainLayer = mainPanel->getLayer();

		if (mainIndex + 1 < Constant::MAX_PACK_INDEX)
		{
			rightPanel = auxPanel;
			rightLayer = rightPanel->getLayer();

			Size visibleSize = Director::getInstance()->getVisibleSize();
			rightLayer->setPositionX(visibleSize.width);
		}

		//recolocar en posiciones iniciales
		mainLayer->setPositionX(mainSpace);
		mainLayer->setScale(mainScale);

		leftLayer->setPositionX(0);
		leftLayer->setScale(mainScale / 1.7);

		auxPanel = NULL;
		auxLayer = NULL;

		leftCount = 0, mainCount = 0, rightCount = 0, auxCount = 0;
		mainCountScale = 0;
		leftFinish = mainFinish = rightFinish = scaleFinish = auxFinish = false;
		mainIndex++;

		log("INDEX MAIN - %d ", mainIndex);
		moveLeft = false;
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