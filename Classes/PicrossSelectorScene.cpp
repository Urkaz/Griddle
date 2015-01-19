#include "PicrossSelectorScene.h"
#include "MainMenuScene.h"
#include <string>
#include "PicrossGameScene.h"

using namespace cocos2d;
using namespace std;

short Global::PUZZLE_NUMBER = 0;
short Global::CURRENT_PACK_INDEX = 0;
const int mainScale = 5;

Sprite* personaje_selector;
Sprite* personaje_selector_cara;

Texture2D::TexParams textparams;

TTFConfig labelParams;
TTFConfig labelParamsTitulo;
Label* labelPersonaje;
Label* packNameLabel;

Menu* menuSelected;

Sprite* PicrossPreview;
Label* picrossNameLabel;
Label* picrossLivesLabel;
Label* picrossTimeLabel;
Label* picrossSizeLabel;

Sprite* gameModeSprite;

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
    
    textparams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
    
	//Inicializar variables
	if (Global::CURRENT_PACK_INDEX == 0)
	{
		mainIndex = 1;
		Global::CURRENT_PACK_INDEX = mainIndex;
	}
	else
	{
		mainIndex = Global::CURRENT_PACK_INDEX;
	}

	//Animacióm
	moveLeft = moveRight = false;
	leftCount = 0, mainCount = 0, rightCount = 0, auxCount = 0;
	mainCountScale = 0;
	leftFinish = mainFinish = rightFinish = scaleFinish = auxFinish = false;
	mov = 0;

	selected = selectEnabled = unselectEnabled = false;

	//Crear listener del ratón
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(PicrossSelectorScene::onMouseDown, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

	//Tamaño pantalla
	Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto background = Sprite::create("fondo_prueba2.png");
    background->setScale(2, 2);
    background->setPosition(visibleSize.width/2, visibleSize.height /2);
    addChild(background, 0);
    
    auto avion = Sprite::create("AvionDestrozado.png");
    avion->setPosition(Point((visibleSize.width  /1.15),
                                  (visibleSize.height /8)));
    addChild(avion, 1);
    
	auto labelbackground = Sprite::create("labeltutorial.png");
    labelbackground->setScale(1.5f, 0.15f);
    
    labelbackground->setPosition(visibleSize.width/2, visibleSize.height /80);
    addChild(labelbackground, 0);

    personaje_selector = Sprite::create("personaje1.png");
	personaje_selector->getTexture()->setTexParameters(textparams);
    personaje_selector->setPosition(Point((visibleSize.width/8), (visibleSize.height/7)));
    personaje_selector->setScale(8);
    addChild(personaje_selector);

	//Imagen que indica el modo de juego
	if (Global::GAMEMODE == GameMode::NORMAL)
		gameModeSprite = Sprite::create("modo_normal.png");
	else if (Global::GAMEMODE == GameMode::FREE)
		gameModeSprite = Sprite::create("modo_libre.png");
	else if (Global::GAMEMODE == GameMode::BOMB)
		gameModeSprite = Sprite::create("modo_bomba.png");

	gameModeSprite->setPosition(gameModeSprite->getBoundingBox().size.width/2, visibleSize.height - gameModeSprite->getBoundingBox().size.height/2);
	this->addChild(gameModeSprite, 1);

	//Posición inicial paneles
	//Main Panel
	mainPanel = new PanelSelector(mainIndex);
	mainLayer = mainPanel->getLayer();

	mainLayer->setScale(mainScale);

	int layerOffsetX = visibleSize.width / 2 + mainLayer->getBoundingBox().size.width / 2;
	int layerOffsetY = visibleSize.height / 2 + mainLayer->getBoundingBox().size.height / 2;

	mainLayer->setPosition(layerOffsetX, layerOffsetY);

	//Panel Derecho
	if (mainIndex + 1 <= Global::MAX_PACK_INDEX && Global::MAX_PACK_INDEX != 1)
	{
		rightPanel = new PanelSelector(mainIndex+1);
		rightLayer = rightPanel->getLayer();

		rightLayer->setScale((float)(mainScale / 1.7));
		rightLayer->setPosition(visibleSize.width + rightLayer->getBoundingBox().size.width/2, mainLayer->getPositionY());

		addChild(rightLayer);
	}
	//Panel Izquierdo
	if (mainIndex - 1 > 0 && Global::MAX_PACK_INDEX != 1)
	{
		leftPanel = new PanelSelector(mainIndex-1);
		leftLayer = leftPanel->getLayer();

		leftLayer->setScale((float)(mainScale / 1.7));
		leftLayer->setPosition(0 + leftLayer->getBoundingBox().size.width / 2, mainLayer->getPositionY());

		addChild(leftLayer);
	}
	addChild(mainLayer);

	//Seleccionar primer picross del panel
	Global::PUZZLE_NUMBER = mainPanel->getFirstPicrossID();

	//Label del diálogo del personaje
	labelParams.fontFilePath = "LondrinaSolid-Regular.otf";
	labelParams.fontSize = 25;

	labelPersonaje = Label::createWithTTF(labelParams, "Elige el tema del panel y luego selecciona un Griddle.");
	labelPersonaje->setPosition(visibleSize.width / 2, visibleSize.height / 25);
	labelPersonaje->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	labelPersonaje->setColor(Color3B(0, 100, 200));
	addChild(labelPersonaje);
	
	//Label título del panel
	labelParamsTitulo.fontFilePath = "LondrinaSolid-Regular.otf";
	labelParamsTitulo.fontSize = 60;

	packNameLabel = Label::createWithTTF(labelParamsTitulo, mainPanel->getPanelName());
	packNameLabel->setPosition(visibleSize.width / 2, visibleSize.height - labelParamsTitulo.fontSize - 10);
	packNameLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	packNameLabel->setColor(Color3B(255, 255, 255));
	addChild(packNameLabel);

	//Botón volver atrás (cambiar) Lo tengo hecho, lo cambiaré
	auto exit = MenuItemImage::create("x_boton.png",
		"x_boton.png",
		CC_CALLBACK_1(PicrossSelectorScene::returnToMainMenu, this));
    
	auto exitMenu = Menu::create(exit,NULL);
    exitMenu->setPosition(visibleSize.width-20, visibleSize.height-20); //cambiar
    addChild(exitMenu, 0);

	//Menu para cuando está seleccionado un panel
	auto playButton = MenuItemImage::create("ganarselector.png",
		"ganarselector.png",
		CC_CALLBACK_1(PicrossSelectorScene::goToPicrossGame, this));
    playButton->setScale(0.1f);

	auto unselectButton = MenuItemImage::create("atrasselector.png",
		"atrasselector.png",
		CC_CALLBACK_1(PicrossSelectorScene::enableUnselect, this));
    unselectButton->setScale(0.1f);

	menuSelected = Menu::create(playButton, unselectButton, NULL);
	menuSelected->setPosition(visibleSize.width * 4 / 5, visibleSize.height * 1.5 / 5);
	menuSelected->alignItemsVerticallyWithPadding(0.3f);
	addChild(menuSelected, 0);

	//Vista previa del picross seleccionado
	PicrossPreview = Sprite::create("empty_selector.png");
	PicrossPreview->setTexture(mainPanel->getFirstPicrossTexture());
	PicrossPreview->setScale(mainScale);
	PicrossPreview->setPosition(visibleSize.width * 4 / 5, visibleSize.height * 3.5 / 5);

	addChild(PicrossPreview);

	//Labels de vista previa
	picrossNameLabel = Label::createWithTTF(labelParams, "????");
	picrossNameLabel->setPosition(PicrossPreview->getPositionX(), PicrossPreview->getPositionY() - 70);
	picrossNameLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	picrossNameLabel->setColor(Color3B(0, 100, 200));

	picrossSizeLabel = Label::createWithTTF(labelParams, "(?x?)");
	picrossSizeLabel->setPosition(PicrossPreview->getPositionX(), PicrossPreview->getPositionY() - 100);
	picrossSizeLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	picrossSizeLabel->setColor(Color3B(0, 100, 200));

	picrossTimeLabel = Label::createWithTTF(labelParams, "Tiempo: ?:??");
	picrossTimeLabel->setPosition(PicrossPreview->getPositionX(), PicrossPreview->getPositionY() - 130);
	picrossTimeLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	picrossTimeLabel->setColor(Color3B(0, 100, 200));

	if (Global::GAMEMODE == GameMode::NORMAL)
	{
		picrossLivesLabel = Label::createWithTTF(labelParams, "Fallos: ?");
		picrossLivesLabel->setPosition(PicrossPreview->getPositionX(), PicrossPreview->getPositionY() - 160);
		picrossLivesLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		picrossLivesLabel->setColor(Color3B(0, 100, 200));

		this->addChild(picrossLivesLabel);
	}

	this->addChild(picrossNameLabel);
	this->addChild(picrossTimeLabel);
	this->addChild(picrossSizeLabel);

	setPreviewData(Global::PUZZLE_NUMBER);
	setPreviewDataVisible(false);

	//Activar el método update
	this->scheduleUpdate();

	return true;
}

void PicrossSelectorScene::setPreviewDataVisible(bool visible)
{
	PicrossPreview->setVisible(visible);
	menuSelected->setVisible(visible);
	picrossNameLabel->setVisible(visible);
	picrossTimeLabel->setVisible(visible);
	picrossSizeLabel->setVisible(visible);
	if (Global::GAMEMODE == GameMode::NORMAL)
		picrossLivesLabel->setVisible(visible);
}

void PicrossSelectorScene::setPreviewData(int num)
{
	//Picross auxiliar para los datos de la vista previa
	auxPicross = new Picross(num, Global::GAMEMODE);

	bool completed = UserDefault::getInstance()->getBoolForKey(("n_" + to_string(Global::PUZZLE_NUMBER)).c_str());

	picrossSizeLabel->setString("(" + to_string(auxPicross->getRowNumber()) + "x" + to_string(auxPicross->getColumnNumber()) + ")");

	if (!completed)
	{
		picrossNameLabel->setString("????");
		picrossTimeLabel->setString("Tiempo: ?:??");
		picrossTimeLabel->setColor(Color3B(0, 100, 200));
		if (Global::GAMEMODE == GameMode::NORMAL)
		{
			picrossLivesLabel->setString("Fallos: ?");
			picrossLivesLabel->setColor(Color3B(0, 100, 200));
		}
	}
	else
	{
		picrossNameLabel->setString(auxPicross->getName());

		//TIEMPO
		int pv_tiempo = UserDefault::getInstance()->getIntegerForKey(("n_" + to_string(Global::PUZZLE_NUMBER) + "_tiempo").c_str());
		picrossTimeLabel->setString("Tiempo " + to_string((int)(pv_tiempo / 60)) + ":" +
			(to_string((int)pv_tiempo % 60).length() < 2 ? "0" + to_string((int)pv_tiempo % 60) : to_string((int)pv_tiempo % 60)));
		if (pv_tiempo < Global::TIME_LIMIT*60)
			picrossTimeLabel->setColor(Color3B(102, 255, 51));
		else
			picrossTimeLabel->setColor(Color3B(245, 51, 0));

		//FALLOS
		if (Global::GAMEMODE == GameMode::NORMAL)
		{
			int pv_fallos = UserDefault::getInstance()->getIntegerForKey(("n_" + to_string(Global::PUZZLE_NUMBER) + "_fallos").c_str());
			picrossLivesLabel->setString("Fallos: " + to_string(pv_fallos));
			if (pv_fallos == 0)
				picrossLivesLabel->setColor(Color3B(102, 255, 51));
			else
				picrossLivesLabel->setColor(Color3B(245, 51, 0));
		}
	}

}

void PicrossSelectorScene::onMouseDown(Event* event)
{
	if (!moveRight && !moveLeft)
	{
		auto* e = (EventMouse*)event;
		float cursorY = e->getCursorY();
		float cursorX = e->getCursorX();

		//RIGHT PANEL
		if (mainIndex + 1 <= Global::MAX_PACK_INDEX  && Global::MAX_PACK_INDEX != 1)
		{
			//Offset del rightPanel respecto al 0,0
			int rightSide = (int)(Global::SELECTOR_SQUARE_SIDE * rightLayer->getScale());
			int rightOffSetX = rightLayer->getPosition().x - 5 / 2 * rightSide - 5 % 2 * rightSide / 2 - rightLayer->getBoundingBox().size.height / 2;
			int rightOffSetY = rightLayer->getPosition().y - 5 / 2 * rightSide - 5 % 2 * rightSide / 2 - rightLayer->getBoundingBox().size.width / 2;

			if (cursorY > rightOffSetY && cursorX > rightOffSetX &&
				cursorY < rightOffSetY + rightSide * 5 &&
				cursorX < rightOffSetX + rightSide * 5)
			{
				//log("TOUCH RIGHT");
				if (!selectEnabled && !selected)
					enableLeftAnim();
			}
		}
		//LEFT PANEL
		if (mainIndex - 1 > 0 && Global::MAX_PACK_INDEX != 1)
		{
			//Offset del leftPanel respecto al 0,0
			int leftSide = (int)(Global::SELECTOR_SQUARE_SIDE * leftLayer->getScale());
			int leftOffSetX = leftLayer->getPosition().x - 5 / 2 * leftSide - 5 % 2 * leftSide / 2 - leftLayer->getBoundingBox().size.height / 2;
			int leftOffSetY = leftLayer->getPosition().y - 5 / 2 * leftSide - 5 % 2 * leftSide / 2 - leftLayer->getBoundingBox().size.width / 2;

			if (cursorY > leftOffSetY && cursorX > leftOffSetX &&
				cursorY < leftOffSetY + leftSide * 5 &&
				cursorX < leftOffSetX + leftSide * 5)
			{
				//log("TOUCH LEFT");
				if (!selectEnabled && !selected)
					enableRightAnim();
			}
		}

		//Offset del mainPanel respecto al 0,0
		int mainSide = (int)(Global::SELECTOR_SQUARE_SIDE * mainLayer->getScale());
		int mainOffSetX = mainLayer->getPosition().x - 5 / 2 * mainSide - 5 % 2 * mainSide / 2 - mainLayer->getBoundingBox().size.height / 2;
		int mainOffSetY = mainLayer->getPosition().y - 5 / 2 * mainSide - 5 % 2 * mainSide / 2 - mainLayer->getBoundingBox().size.width / 2;

		//MAIN PANEL
		if (cursorY > mainOffSetY && cursorX > mainOffSetX &&
			cursorY < mainOffSetY + mainSide * 5 &&
			cursorX < mainOffSetX + mainSide * 5)
		{
			if (!moveLeft && !moveRight && !selectEnabled && !unselectEnabled)
			{
				//Coordenadas fila(i),columna(j)
				short i = (cursorY - mainOffSetY) / mainSide;
				short j = (cursorX - mainOffSetX) / mainSide;

				i = std::abs(i - 5 + 1);

				//log("ROW:(i): %d , COL(j) %d",i,j);

				int id = mainPanel->getPicrossID(i, j);

				if (id != 0)
				{
					PicrossPreview->setTexture(mainPanel->getPicrossTextureIndex(i, j));

					Global::PUZZLE_NUMBER = id;

					setPreviewData(Global::PUZZLE_NUMBER);

					log("PICROSS SELECCIONADO: %d", Global::PUZZLE_NUMBER);
				}

				if (!selected)
					enableSelect();
			}
		}
	}
}

void PicrossSelectorScene::update(float dt)
{
	if (moveLeft)
	{
		movePanelsToLeft(dt);
	}
	else if (moveRight)
	{
		movePanelsToRight(dt);
	}
	else if (selectEnabled)
	{
		selectPanel(dt);
	}
	else if (unselectEnabled)
	{
		unselectPanel(dt);
	}
}

void PicrossSelectorScene::enableLeftAnim()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//desplazamientos totales a realizar
	sideSpace = Global::SELECTOR_SQUARE_SIDE * mainScale / 1.7 * 5 / 2;
	mainSpace = mainLayer->getPositionX();
	mainScaleSpace = (float)(mainScale - mainScale / 1.7);

	if (mainIndex + 2 <= Global::MAX_PACK_INDEX)
	{
		auxPanel = new PanelSelector(mainIndex + 2);
		auxLayer = auxPanel->getLayer();
		auxLayer->setPosition(visibleSize.width + sideSpace, rightLayer->getPositionY());
		auxLayer->setScale(rightLayer->getScale());
		addChild(auxLayer);
	}
	if (mainIndex + 1 <= Global::MAX_PACK_INDEX)
	{
		moveLeft = true;
	}
}

void PicrossSelectorScene::movePanelsToLeft(float dt)
{
	mov = 300 * dt;

	//Izquiero sale de la pantalla
	if (sideSpace > leftCount + mov && mainIndex - 1 > 0)
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
	if (scaleFinish)
	{
		if (mainIndex + 2 <= Global::MAX_PACK_INDEX)
		{
			if (sideSpace > auxCount + mov)
			{
				auxLayer->setPositionX(auxLayer->getPositionX() - mov);
				auxCount += mov;
			}
			else auxFinish = true;
		}
		else auxFinish = true;
	}

	//Todos han terminado = reiniciar variables y desactivar animación.
	if (leftFinish && mainFinish && rightFinish && auxFinish)
	{
		//Eliminar el que se ha ocultado
		removeChild(leftLayer);

		//Swap de paneles y variables
		leftPanel = mainPanel;
		leftLayer = leftPanel->getLayer();

		mainPanel = rightPanel;
		mainLayer = mainPanel->getLayer();

		if (mainIndex + 1 < Global::MAX_PACK_INDEX)
		{
			rightPanel = auxPanel;
			rightLayer = rightPanel->getLayer();

			Size visibleSize = Director::getInstance()->getVisibleSize();
			rightLayer->setPositionX(visibleSize.width);
		}
		else
		{
			rightPanel = NULL;
			rightLayer = NULL;
		}

		//Recolocar en posiciones iniciales
		mainLayer->setPositionX(mainSpace);
		mainLayer->setScale(mainScale);

		leftLayer->setPositionX(0);
		leftLayer->setScale((float)(mainScale / 1.7));

		auxPanel = NULL;
		auxLayer = NULL;

		packNameLabel->setString(mainPanel->getPanelName());
		PicrossPreview->setTexture(mainPanel->getFirstPicrossTexture());
		Global::PUZZLE_NUMBER = mainPanel->getFirstPicrossID();

		leftCount = 0, mainCount = 0, rightCount = 0, auxCount = 0;
		mainCountScale = 0;
		leftFinish = mainFinish = rightFinish = scaleFinish = auxFinish = false;
		mainIndex++;

		//log("INDEX MAIN - %d ", mainIndex);
		moveLeft = false;
	}
}

void PicrossSelectorScene::enableRightAnim()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//Desplazamientos totales a realizar
	sideSpace = Global::SELECTOR_SQUARE_SIDE * mainScale / 1.7 * 5 / 2;
	mainSpace = mainLayer->getPositionX();
	mainScaleSpace = (float)(mainScale - mainScale / 1.7);

	if (mainIndex - 2 > 0)
	{
		auxPanel = new PanelSelector(mainIndex - 2);
		auxLayer = auxPanel->getLayer();
		auxLayer->setPosition(-sideSpace, leftLayer->getPositionY());
		auxLayer->setScale(leftLayer->getScale());
		addChild(auxLayer);
	}
	if (mainIndex - 1 != 0)
	{
		moveRight = true;
		//log("RIGHT MOV");
	}
}

void PicrossSelectorScene::movePanelsToRight(float dt)
{
	mov = 300 * dt;

	//Izquierdo se va al medio
	if (mainSpace > leftCount - mov*1.3)
	{
		leftLayer->setPositionX(leftLayer->getPositionX() + mov*1.3);
		leftCount += mov*1.3;
	}
	else leftFinish = true;

	//Principal se va a la izquerda
	if (mainSpace > mainCount + mov*1.3)
	{
		mainLayer->setPositionX(mainLayer->getPositionX() + mov*1.3);
		mainCount += mov * 1.3;
	}
	else mainFinish = true;
	
	//Derecho se sale de la pantalla
	if (sideSpace > rightCount + mov && mainIndex + 1 <= Global::MAX_PACK_INDEX)
	{
		rightLayer->setPositionX(rightLayer->getPositionX() + mov);
		rightCount += mov;
	}
	else rightFinish = true;

	//SCALE
	if (mainScaleSpace > mainCountScale + 2 * dt)
	{
		mainLayer->setScale(mainLayer->getScale() - 2 * dt);
		leftLayer->setScale(leftLayer->getScale() + 2 * dt);

		mainCountScale += 2 * dt;
	}
	else scaleFinish = true;

	//El nuevo panel entra en pantalla
	if (scaleFinish)
	{
		if (mainIndex - 2 > 0)
		{
			if (sideSpace > auxCount + mov)
			{
				auxLayer->setPositionX(auxLayer->getPositionX() + mov);
				auxCount += mov;
			}
			else auxFinish = true;
		}
		else auxFinish = true;
	}

	//Todos han terminado = reiniciar variables y desactivar animación.
	if (leftFinish && mainFinish && rightFinish && auxFinish)
	{
		//Eliminar el que se ha ocultado
		removeChild(rightLayer);

		//Swap de paneles y variables
		rightPanel = mainPanel;
		rightLayer = rightPanel->getLayer();

		mainPanel = leftPanel;
		mainLayer = mainPanel->getLayer();

		if (mainIndex - 2 > 0)
		{
			leftPanel = auxPanel;
			leftLayer = leftPanel->getLayer();

			leftLayer->setPositionX(0);
			leftLayer->setScale((float)(mainScale / 1.7));
		}
		else
		{
			leftPanel = NULL;
			leftLayer = NULL;
		}

		Size visibleSize = Director::getInstance()->getVisibleSize();
		rightLayer->setPositionX(visibleSize.width);

		//Recolocar en posiciones iniciales
		mainLayer->setPositionX(mainSpace);
		mainLayer->setScale(mainScale);

		auxPanel = NULL;
		auxLayer = NULL;

		packNameLabel->setString(mainPanel->getPanelName());
		PicrossPreview->setTexture(mainPanel->getFirstPicrossTexture());
		Global::PUZZLE_NUMBER = mainPanel->getFirstPicrossID();

		leftCount = 0, mainCount = 0, rightCount = 0, auxCount = 0;
		mainCountScale = 0;
		leftFinish = mainFinish = rightFinish = scaleFinish = auxFinish = false;
		mainIndex--;

		moveRight = false;
	}
}

void PicrossSelectorScene::goToPicrossGame(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("cambio_escena.wav");

	auto scene = PicrossGameScene::createScene();

	Director::getInstance()->pushScene(TransitionFadeTR::create(1, scene));
}

void PicrossSelectorScene::returnToMainMenu(Ref *pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("cambio_escena.wav");

	Global::CURRENT_PACK_INDEX = mainIndex;

	auto scene = MainMenuScene::createScene();

	Director::getInstance()->replaceScene(TransitionFadeBL::create(1, scene));
}

void PicrossSelectorScene::enableSelect()
{
	sideSpace = Global::SELECTOR_SQUARE_SIDE * mainScale / 1.7 * 5 / 2+5;
	mainSpace = sideSpace;

	selectEnabled = true;
}

void PicrossSelectorScene::enableUnselect(Ref *pSender)
{
	setPreviewDataVisible(false);

	sideSpace = Global::SELECTOR_SQUARE_SIDE * mainScale / 1.7 * 5 / 2+5;
	mainSpace = sideSpace;

	unselectEnabled = true;
}

void PicrossSelectorScene::selectPanel(float dt)
{
	mov = 300 * dt;

	//Principal se mueve un poco hacia la izquierda
	if (mainSpace > mainCount + mov*1.3)
	{
		mainLayer->setPositionX(mainLayer->getPositionX() - mov*1.3);
		mainCount += mov * 1.3;
	}
	else mainFinish = true;

	//Izquiero sale de la pantalla
	if (sideSpace > leftCount + mov*1.3 && mainIndex - 1 > 0)
	{
		leftLayer->setPositionX(leftLayer->getPositionX() - mov*1.3);
		leftCount += mov*1.3;
	}
	else leftFinish = true;

	//Derecho se sale de la pantalla
	if (sideSpace > rightCount + mov*1.3 && mainIndex + 1 <= Global::MAX_PACK_INDEX)
	{
		rightLayer->setPositionX(rightLayer->getPositionX() + mov*1.3);
		rightCount += mov*1.3;
	}
	else rightFinish = true;

	//Todos han terminado = reiniciar variables y desactivar animación.
	if (leftFinish && mainFinish && rightFinish)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		//Recolocar en posiciones iniciales
		if (mainIndex - 1 > 0)
		{
			leftLayer->setPositionX(-sideSpace);
		}
		if (mainIndex + 1 <= Global::MAX_PACK_INDEX)
		{
			rightLayer->setPositionX(visibleSize.width+sideSpace);
		}
		mainLayer->setPositionX(visibleSize.width/2-mainSpace);
		
		setPreviewData(Global::PUZZLE_NUMBER);
		setPreviewDataVisible(true);

		leftCount = 0, mainCount = 0, rightCount = 0;
		leftFinish = mainFinish = rightFinish = false;
		selectEnabled = false;
		selected = true;
	}
}

void PicrossSelectorScene::unselectPanel(float dt)
{
	mov = 300 * dt;

	//Principal se mueve un poco hacia la derecha
	if (mainSpace > mainCount + mov*1.3)
	{
		mainLayer->setPositionX(mainLayer->getPositionX() + mov*1.3);
		mainCount += mov * 1.3;
	}
	else mainFinish = true;

	//Izquiero entra en la pantalla
	if (sideSpace > leftCount + mov*1.3 && mainIndex - 1 > 0)
	{
		leftLayer->setPositionX(leftLayer->getPositionX() + mov*1.3);
		leftCount += mov*1.3;
	}
	else leftFinish = true;

	//Derecho entra en la pantalla
	if (sideSpace > rightCount + mov*1.3 && mainIndex + 1 <= Global::MAX_PACK_INDEX)
	{
		rightLayer->setPositionX(rightLayer->getPositionX() - mov*1.3);
		rightCount += mov*1.3;
	}
	else rightFinish = true;

	//Todos han terminado = reiniciar variables y desactivar animación.
	if (leftFinish && mainFinish && rightFinish)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		//Recolocar en posiciones iniciales
		if (mainIndex - 1 > 0)
		{
			leftLayer->setPositionX(0);
		}
		if (mainIndex + 1 <= Global::MAX_PACK_INDEX)
		{
			rightLayer->setPositionX(visibleSize.width);
		}
		mainLayer->setPositionX(visibleSize.width / 2);

		leftCount = 0, mainCount = 0, rightCount = 0;
		leftFinish = mainFinish = rightFinish = false;
		unselectEnabled = false;
		selected = false;
	}
}