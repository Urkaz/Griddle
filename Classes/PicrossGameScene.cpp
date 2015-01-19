#include "PicrossGameScene.h"
#include "Global.h"
#include "PauseScene.h"
#include "EndScene.h"
#include "WinScene.h"


using namespace cocos2d;
using namespace std;

short Global::PICROSS_SQUARE_SIDE;
short Global::FONT_SIZE;
short Global::LIFE;
short Global::TIME;
WinScene* Winscene;

Texture2D* texMarkX;
Texture2D* texDraw;
Texture2D* texEmpty;
Texture2D* texButtonDraw;
Texture2D* texButtonMarkX;
Texture2D* texButtonClickDraw;
Texture2D* texButtonClickMarkX;
Texture2D* texButtonClickPause;
Texture2D* texDeco;

Texture2D::TexParams textureParams;

Sprite* button_draw;
Sprite* button_X;

TTFConfig fontLabelConfig;
TTFConfig numbersLabelConfig;
Label* lifeLabel;
Label* timeLabel;

int initialScale;

bool mouseDown;
float lastCursorX, lastCursorY;


Scene* PicrossGameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = PicrossGameScene::create();
	scene->addChild(layer);
	return scene;
}

bool PicrossGameScene::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
   
	//Crear listener del ratón
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(PicrossGameScene::onMouseDown, this);
	mouseListener->onMouseMove = CC_CALLBACK_1(PicrossGameScene::onMouseMove, this);
	mouseListener->onMouseUp = CC_CALLBACK_1(PicrossGameScene::onMouseUp, this);
	mouseDown = false;

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(PicrossGameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(PicrossGameScene::onKeyReleased, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	//Carga de texturas
	textureParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	texMarkX = Director::getInstance()->getTextureCache()->addImage("markX_picross.png");
	texDraw = Director::getInstance()->getTextureCache()->addImage("draw_picross.png");
	texEmpty = Director::getInstance()->getTextureCache()->addImage("empty_picross.png");

	texButtonDraw = Director::getInstance()->getTextureCache()->addImage("boton_lapiz.png");
	texButtonMarkX = Director::getInstance()->getTextureCache()->addImage("boton_X.png");
	texButtonClickDraw = Director::getInstance()->getTextureCache()->addImage("boton_click_lapiz.png");
	texButtonClickMarkX = Director::getInstance()->getTextureCache()->addImage("boton_click_X.png");

	texDeco = Director::getInstance()->getTextureCache()->addImage("fuera_picross.png");

    texMarkX->setTexParameters(textureParams);
	texDraw->setTexParameters(textureParams);
	texEmpty->setTexParameters(textureParams);

	texDeco->setTexParameters(textureParams);

	tiempo_ms = 0;

	//Se crea un Picross basado en los parámetros elegidos durante las pantallas de selección
	picross = new Picross(Global::PUZZLE_NUMBER, Global::GAMEMODE);

	solutionNum = picross->getSolutionNum();
	userSquareNum = 0;

	//Inicializar matriz de solución del usuario
	userSolution = vector<vector<int>>(picross->getRowNumber());
	for(int i = 0; i < picross->getRowNumber(); i++)
	{
		vector<int> row = vector<int>(picross->getColumnNumber());
		for(int j = 0; j < picross->getColumnNumber(); j++)
		{
			row[j] = 0;
		}
		userSolution[i] = row;
	}

	//Se comprueba el GameMode para determinar el modo de creación de la matriz.
	if(Global::GAMEMODE != GameMode::TRIANGLES)
	{
		initialScale = 5;

		picrossGridVector = createSquareMatrix(picross);
		picrossGridLayer = createGridLayer(picrossGridVector);

		//Se define la constante del lado de cada cuadrado del tablero.
		Global::PICROSS_SQUARE_SIDE = picrossGridVector[0][0]->getBoundingBox().size.width*picrossGridLayer->getScale();

		//Config del label de numeros
		numbersLabelConfig.fontFilePath = "LondrinaSolid-Regular.otf";
		Global::FONT_SIZE = Global::PICROSS_SQUARE_SIDE*7/8;
		numbersLabelConfig.fontSize = Global::FONT_SIZE;
		numbersLabelConfig.distanceFieldEnabled = true;

		//Filas y columnas
		rowNumbers = generateNumbers(picross, false);
		columnNumbers = generateNumbers(picross, true);

		numbersLayer = createSquareNumbersLayer(rowNumbers, columnNumbers);

		//Decoración
		createDecoration(picrossGridVector, picrossGridLayer, picross);


		if (picross->getRowNumber() >= picross->getColumnNumber())
		{
			log("GRD %f", picrossGridLayer->getScale() - picross->getRowNumber() / initialScale);
			log("NUM %f", numbersLayer->getScale() - 0.2f * (picross->getRowNumber() / initialScale));

			picrossGridLayer->setScale(picrossGridLayer->getScale() - picross->getRowNumber() / initialScale);
			numbersLayer->setScale(numbersLayer->getScale() - 0.2f * (picross->getRowNumber() / initialScale));
			Global::PICROSS_SQUARE_SIDE = picrossGridVector[0][0]->getBoundingBox().size.width*picrossGridLayer->getScale();
		}
		else
		{
			picrossGridLayer->setScale(picrossGridLayer->getScale() - picross->getColumnNumber() / initialScale);
			numbersLayer->setScale(numbersLayer->getScale() - 0.2f * (picross->getColumnNumber() / initialScale));
			Global::PICROSS_SQUARE_SIDE = picrossGridVector[0][0]->getBoundingBox().size.width*picrossGridLayer->getScale();
		}
	}
	//NO IMPLEMENTADO
	/*else
		picrossGridVector = createTriangleGrid();*/
	
	//Botón de pausa
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto pauseItem = MenuItemImage::create("boton_pausa.png",
		"boton_click_pausa.png",
		CC_CALLBACK_1(PicrossGameScene::goToPauseScene, this));

	auto pauseMenu = Menu::create(pauseItem, NULL);

	pauseMenu->setPosition(visibleSize.width - pauseItem->getBoundingBox().size.width / 2, visibleSize.height - pauseItem->getBoundingBox().size.height / 2);

   
    auto background = Sprite::create("fondo_prueba.png");
    background->setScale(2, 2);
    background->setPosition(visibleSize.width/2, visibleSize.height /2);
    addChild(background, 0);
    
    
	//Botones pintar y marcar X
	drawEnabled = true;
	markXEnabled = false;
	button_draw = Sprite::create("boton_click_lapiz.png");
	button_X = Sprite::create("boton_X.png");
	button_draw->setPosition(button_draw->getBoundingBox().size.width/2,visibleSize.height/2+button_draw->getBoundingBox().size.height/2);
	button_X->setPosition(button_X->getBoundingBox().size.width/2,button_X->getBoundingBox().size.height/2);

	//Config fuente
	fontLabelConfig.fontFilePath = "LondrinaSolid-Regular.otf";
	fontLabelConfig.fontSize = 25;

	//Tiempo
	timeLabel = Label::createWithTTF(fontLabelConfig, "0:00");
	timeLabel->setPosition(pauseMenu->getPositionX()-5, pauseMenu->getPositionY() - 45);
	timeLabel->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
	this->addChild(timeLabel, 1);

	//Se muestran las vidas
	if (Global::GAMEMODE == GameMode::NORMAL)
	{
		lifes = 3;
		lifeLabel = Label::createWithTTF(fontLabelConfig, "Vidas " + to_string(lifes));
		lifeLabel->setPosition(timeLabel->getPositionX()-18, timeLabel->getPositionY() - 30);
		lifeLabel->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
		this->addChild(lifeLabel, 1);
	}

	//Se dibuja el tablero
	this->addChild(button_draw, 1);
	this->addChild(button_X, 1);
	this->addChild(pauseMenu, 1);

	this->addChild(picrossGridLayer, 0);
	this->addChild(numbersLayer, 0);

	//Activar el método update
	this->scheduleUpdate();

	return true;
}

/**
 * Crea la matriz de Sprites que forman un Picross cuadrado.
 * ----
 * vector[i][j]
 * "i" representa el número de fila
 * "j" representa el número de columna dentro de la fila "i"
 * ----
 * vector[0] = fila 0;
 * vector[0][0] = fila 0, columna 0.
 * vector[1][2] = fila 1, columna 2.
 * ----
 * @param   picross	Instancia de la clase Picross.
 * @return  Vector de filas de Sprites del Picross.
 */
vector<vector<Sprite*>> PicrossGameScene::createSquareMatrix(Picross* picross)
{
	vector<vector<Sprite*>> matrix = vector<vector<Sprite*>>(picross->getRowNumber());

	for(int i = 0; i < picross->getRowNumber(); i++) //"i" representa el número de fila
	{
		//Se crea una nueva fila a la que añadir sprites
		vector<Sprite*> row = vector<Sprite*>(picross->getColumnNumber());
		for(int j = 0; j < picross->getColumnNumber(); j++) //"j" representa el número de columna dentro de la fila "i"
		{
			//Se crea un sprite con la imagen por defecto de vacío.
			Sprite* sprite = Sprite::create("empty_picross.png");
			sprite->getTexture()->setTexParameters(textureParams);

			//Se signa la posición al sprite relativa a la cuadrícula
			int spriteOffsetX = sprite->getBoundingBox().size.width / 2 - sprite->getBoundingBox().size.width*picross->getColumnNumber() / 2;
			int spriteOffsetY = -sprite->getBoundingBox().size.height / 2 + sprite->getBoundingBox().size.height*picross->getRowNumber() / 2;

			int posX = spriteOffsetX + sprite->getBoundingBox().size.width*j;
			int posY = spriteOffsetY + sprite->getBoundingBox().size.height*-i;

			sprite->setPosition(posX,posY);

			//Se añade a la fila
			row[j] = sprite;
		}
		//La fila entera se añade a la matriz
		matrix[i] = row;
	}

	return matrix;
}

/**
 * Crea una única capa que contiene todos los Sprites de un Picross de cualquier tipo.
 *
 * @param   spriteVector	Vector de filas de Sprites de un Picross de cualquier tipo.
 * @return  Layer con todos los Sprites que forman un Picross centrada en el medio de la pantalla.
 */
Layer* PicrossGameScene::createGridLayer(vector<vector<Sprite*>> spriteVector)
{
	//Se crea una capa en el centro de la pantalla a la que se le añadiran los sprites
	Layer* spriteLayer = Layer::create();
	spriteLayer->setContentSize(Size(1, 1));
	spriteLayer->setScale(initialScale);

	//Se establece la posición
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int layerOffsetX = visibleSize.width  /2 + spriteLayer->getBoundingBox().size.width/2;
	int layerOffsetY = visibleSize.height /2 + spriteLayer->getBoundingBox().size.height/2;

	spriteLayer->setPosition(layerOffsetX, layerOffsetY);

	for(int i = 0; i < (int)spriteVector.size(); i++) //Fila
		for (int j = 0; j < (int)spriteVector[i].size(); j++) //Columna
			spriteLayer->addChild(spriteVector[i][j],0);

	return spriteLayer;
}

void PicrossGameScene::createDecoration(vector<vector<Sprite*>> picrossGridVector, cocos2d::Layer* picrossLayer, Picross* picross)
{
	int offsetX = picross->getColumnNumber() / 2 * picrossGridVector[0][0]->getBoundingBox().size.width - (picrossGridVector[0].size() + 1) % 2 * picrossGridVector[0][0]->getBoundingBox().size.width / 2;
	int offsetY = 2 + picross->getRowNumber() / 2 * picrossGridVector[0][0]->getBoundingBox().size.height + picrossGridVector[0].size() % 2 * picrossGridVector[0][0]->getBoundingBox().size.height / 2;

	for (int i = 0; i < picross->getColumnNumber(); i++)
	{
		Sprite* deco = Sprite::create("fuera_picross.png");

		deco->setTexture(texDeco);
		deco->setPosition(-offsetX + deco->getBoundingBox().size.width*i, -offsetY);

		picrossLayer->addChild(deco);
	}
}

vector<vector<Label*>> PicrossGameScene::generateNumbers(Picross* picross, bool columnsEnabled)
{
	//Según columnsEnabled esté a true o false, se invertirán o no las filas y columnas
	int rows, cols;
	if (!columnsEnabled)
	{
		rows = picross->getRowNumber();
		cols = picross->getColumnNumber();
	}
	else
	{
		rows = picross->getColumnNumber();
		cols = picross->getRowNumber();
	}

	//Generar etiquetas de texto, cada una con un sólo número
	int value, count = 0;

	vector<vector<Label*>> nums = vector<vector<Label*>>(rows);
	for(int i = 0; i < (int)nums.size(); i++)
	{
		vector<Label*> individual = vector<Label*>();
		for (int j = 0; j < cols; j++)
		{
			//intercambiar filas por columnas
			if (!columnsEnabled)
				value = picross->getSolution()[i][j];
			else
				value = picross->getSolution()[j][i];

			//contar cantidad de unos
			if (value == 1)
			{
				count++;
		    }
			else
			{
				if (count > 0) // Si encuentra un 0 por el medio crea una etiqueta y reinicia el contador
				{
					Label* label = Label::createWithTTF(numbersLabelConfig, to_string(count) + " ");
					individual.push_back(label);
					count = 0;
				}
			}
		}

		// Marca las filas o columnas del Picross con una X si no hay ningun 1
		if (individual.size() == 0 && count == 0)
		{
			for (int j = 0; j < cols; j++)
			{
				if (!columnsEnabled)
				{
					userSolution[i][j] = -1;
					picrossGridVector[i][j]->setTexture(texMarkX);
				}
				else
				{
					userSolution[j][i] = -1;
					picrossGridVector[j][i]->setTexture(texMarkX);
				}
			}
		}

		//Para cuando haya una fila toda de 1 y ningun 0; O no haya ningun 1.
		if (individual.size() == 0 || count != 0)
		{
			Label* label = Label::createWithTTF(numbersLabelConfig, to_string(count) + " ");
			individual.push_back(label);
		}

		//Reinicia contador para empezar de nuevo
		count = 0;
		nums[i] = individual;
	}

	return nums;
}

Layer* PicrossGameScene::createSquareNumbersLayer(vector<vector<Label*>> rows, vector<vector<Label*>> columns)
{
	Layer* numbers = Layer::create();
	numbers->setContentSize(Size(1, 1));

	int rowOffsetX = Global::FONT_SIZE / 2 - picrossGridVector[0].size() / 2 * Global::PICROSS_SQUARE_SIDE - picrossGridVector[0].size() % 2 * Global::PICROSS_SQUARE_SIDE / 2;
	int rowOffsetY = picrossGridVector.size() / 2 * Global::PICROSS_SQUARE_SIDE - (picrossGridVector.size() + 1) % 2 * Global::PICROSS_SQUARE_SIDE / 2;

	int colOffsetX = Global::PICROSS_SQUARE_SIDE / 2 - picrossGridVector[0].size() / 2 * Global::PICROSS_SQUARE_SIDE - picrossGridVector[0].size() % 2 * Global::PICROSS_SQUARE_SIDE / 2;
	int colOffsetY = Global::FONT_SIZE + picrossGridVector.size() / 2 * Global::PICROSS_SQUARE_SIDE - (picrossGridVector.size() + 1) % 2 * Global::PICROSS_SQUARE_SIDE / 2;

	//FILAS
	for (int i = 0; i < (int)rows.size(); i++)
	{
		for (int j = 0; j < (int)rows[i].size(); j++)
		{
			rows[i][j]->setPosition(rowOffsetX + Global::FONT_SIZE * (j - (int)rows[i].size()),
				rowOffsetY - (Global::FONT_SIZE + (Global::PICROSS_SQUARE_SIDE - Global::FONT_SIZE)) * i);
			numbers->addChild(rows[i][j]);
		}
	}

	//COLUMNAS
	for (int i = 0; i < (int)columns.size(); i++)
	{
		for (int j = 0; j < (int)columns[i].size(); j++)
		{
			columns[i][j]->setPosition(colOffsetX + Global::PICROSS_SQUARE_SIDE * i,
				colOffsetY + Global::FONT_SIZE * (columns[i].size() - (j + 1)));
			numbers->addChild(columns[i][j]);
		}
	}

	numbers->setPosition(picrossGridLayer->getPosition().x, picrossGridLayer->getPosition().y);
	return numbers;
}

void PicrossGameScene::onMouseDown(Event* event)
{
	auto* e = (EventMouse*)event;
	float cursorY = e->getCursorY();
	float cursorX = e->getCursorX();

	//Offset del tablero respecto al 0,0
	int offSetX = picrossGridLayer->getPosition().x - picrossGridVector[0].size()/2 * Global::PICROSS_SQUARE_SIDE - picrossGridVector[0].size()%2 * Global::PICROSS_SQUARE_SIDE/2 - picrossGridLayer->getBoundingBox().size.height/2;
	int offSetY = picrossGridLayer->getPosition().y - picrossGridVector.size()/2 * Global::PICROSS_SQUARE_SIDE - picrossGridVector.size()%2 * Global::PICROSS_SQUARE_SIDE/2 - picrossGridLayer->getBoundingBox().size.width/2;

	//Coordenadas fila(i),columna(j)
	short i = (cursorY-offSetY)/Global::PICROSS_SQUARE_SIDE;
	short j = (cursorX-offSetX)/Global::PICROSS_SQUARE_SIDE;

	i = std::abs(i-picross->getRowNumber()+1);

	//log("ROW:(i): %d , COL(j) %d",i,j);

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//Zona de pulsación de los botones de pintar y poner X
	if(cursorX < button_draw->getBoundingBox().size.width) //Ancho
	{
		if(cursorY < visibleSize.height/2) //Abajo
		{
			if(!markXEnabled)
			{
				button_X->setTexture(texButtonClickMarkX);
				button_draw->setTexture(texButtonDraw);
				markXEnabled = true;
				drawEnabled = false;
			}
			else
			{
				button_X->setTexture(texButtonMarkX);
				markXEnabled = false;
			}
		}
		else //Arriba
		{
			if(!drawEnabled)
			{
				button_X->setTexture(texButtonMarkX);
				button_draw->setTexture(texButtonClickDraw);
				markXEnabled = false;
				drawEnabled = true;
			}
			else
			{
				button_draw->setTexture(texButtonDraw);
				drawEnabled = false;
			}
		}
	}

	//Dentro del tablero
	if(cursorY > offSetY && cursorX > offSetX &&
		cursorY < offSetY+Global::PICROSS_SQUARE_SIDE*picrossGridVector.size() &&
		cursorX < offSetX+Global::PICROSS_SQUARE_SIDE*picrossGridVector[0].size())
	{
		//Pintar
		if(drawEnabled && !markXEnabled)
		{
			switch(userSolution[i][j])
			{
			case -1: //Hay una X en la casilla -> vaciar
				userSolution[i][j] = 0;
				picrossGridVector[i][j]->setTexture(texEmpty);
				break;
			case 0: //La casilla está vacía -> pintar
				//Modo normal y encuentra un error -> poner una X y restar vida
				if (Global::GAMEMODE == GameMode::NORMAL && picross->getSolution()[i][j] == 0)
				{
					userSolution[i][j] = -1;
					picrossGridVector[i][j]->setTexture(texMarkX);

					//if (!Global::DEBUG)
						lifes -= 1;

					lifeLabel->setString("Vidas " + to_string(lifes));

					if (lifes == 0){
						goToEndScene(this);
					}
				}
				//Modo bomba y encuentra un error -> poner X y "explotar"
				else if (Global::GAMEMODE == GameMode::BOMB && picross->getSolution()[i][j] == 0)
				{
					userSolution[i][j] = -1;
					picrossGridVector[i][j]->setTexture(texMarkX);

					int randRow = rand() % (picross->getRowNumber() - 0 + 1) + 0;
					int randCol = rand() % (picross->getColumnNumber() - 0 + 1) + 0;

					//Rango variable según la cantidad de filas/columnas
					int radius = max(picross->getRowNumber(), picross->getColumnNumber()) * 1 / 4;

					//log("EXPLOSION EN (%d,%d). RADIO DE %d CASILLAS", randRow, randCol, radius);

					//Explosión
					for (int i = -radius; i <= radius; i++)
					{
						for (int j = -radius; j <= radius; j++)
						{
							if (abs(i) + abs(j) <= radius)
							{
								if (randRow + i >= 0 && randRow + i < picross->getRowNumber() &&
									randCol + j >= 0 && randCol + j < picross->getColumnNumber())
								{
									//Si la casilla a borrar estaba pintada, restar para el recuento de la solución
									if (userSolution[randRow + i][randCol + j] == 1)
										userSquareNum -= 1;

									userSolution[randRow + i][randCol + j] = 0;
									picrossGridVector[randRow + i][randCol + j]->setTexture(texEmpty);
								}
							}
						}
					}
				}
				else
				{
					userSolution[i][j] = 1;
					picrossGridVector[i][j]->setTexture(texDraw);
					userSquareNum += 1;
					if (userSquareNum == solutionNum)
					if (rightSquare(picross, userSolution) == true){
						goToWinScene(this);
					}
				}
				break;
			case 1: //La casilla está pintada -> vaciar
				userSolution[i][j] = 0;
				picrossGridVector[i][j]->setTexture(texEmpty);
				userSquareNum -= 1;
				if (userSquareNum == solutionNum)
					if (rightSquare(picross, userSolution) == true)
						goToWinScene(this);
				break;
			}
		}
		//Marcar X
		else if(!drawEnabled && markXEnabled)
		{
			switch(userSolution[i][j])
			{
			case -1: //Hay una X en la casilla -> vaciar
				userSolution[i][j] = 0;
				picrossGridVector[i][j]->setTexture(texEmpty);
				break;
			case 0: //La casilla está vacía -> poner X
				userSolution[i][j] = -1;
				picrossGridVector[i][j]->setTexture(texMarkX);
				break;
			case 1: //La casilla está pintada -> vaciar
				userSolution[i][j] = 0;
				picrossGridVector[i][j]->setTexture(texEmpty);
				userSquareNum -= 1;
				if (userSquareNum == solutionNum)
					if (rightSquare(picross, userSolution) == true)
						goToWinScene(this);
				break;
			}
		}
		else
		{
			lastCursorX = cursorX;
			lastCursorY = cursorY;
			mouseDown = true;
		}
	}
	else
	{
		lastCursorX = cursorX;
		lastCursorY = cursorY;
		mouseDown = true;
	}
}

void PicrossGameScene::onMouseUp(Event* event)
{
	mouseDown = false;
}

void PicrossGameScene::update(float dt)
{
	tiempo_ms += dt;

	timeLabel->setString(to_string((int)(tiempo_ms / 60)) + ":" +
		(to_string((int)tiempo_ms % 60).length() < 2 ? "0" + to_string((int)tiempo_ms % 60) : to_string((int)tiempo_ms % 60)));
}

void PicrossGameScene::onMouseMove(Event* event)
{
	auto* e = (EventMouse*)event;
	float cursorY = e->getCursorY();
	float cursorX = e->getCursorX();

	//Offset del tablero respecto al 0,0
	int offSetX = picrossGridLayer->getPosition().x - picrossGridVector[0].size() / 2 * Global::PICROSS_SQUARE_SIDE - picrossGridVector[0].size() % 2 * Global::PICROSS_SQUARE_SIDE / 2 - picrossGridLayer->getBoundingBox().size.height / 2;
	int offSetY = picrossGridLayer->getPosition().y - picrossGridVector.size() / 2 * Global::PICROSS_SQUARE_SIDE - picrossGridVector.size() % 2 * Global::PICROSS_SQUARE_SIDE / 2 - picrossGridLayer->getBoundingBox().size.width / 2;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//MOVER
	if (mouseDown)
	{
		if (picrossGridLayer->getPositionX() - (lastCursorX - cursorX) < visibleSize.width * 9 / 10 && picrossGridLayer->getPositionX() - (lastCursorX - cursorX) > visibleSize.width * 1.5 / 10)
		{
			picrossGridLayer->setPositionX(picrossGridLayer->getPositionX() - (lastCursorX - cursorX));
			numbersLayer->setPositionX(picrossGridLayer->getPositionX());
		}
		if (picrossGridLayer->getPositionY() - (lastCursorY - cursorY) < visibleSize.height * 9 / 10 && picrossGridLayer->getPositionY() - (lastCursorY - cursorY) > visibleSize.height / 10)
		{
			picrossGridLayer->setPositionY(picrossGridLayer->getPositionY() - (lastCursorY - cursorY));
			numbersLayer->setPositionY(picrossGridLayer->getPositionY());
		}

		lastCursorX = cursorX;
		lastCursorY = cursorY;
	}
}

void PicrossGameScene::goToPauseScene(Ref *pSender)
{
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

    auto scene = PauseScene::createScene();
	Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
    
}

void PicrossGameScene::goToEndScene(Ref *pSender) {


	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/ButtonClick.wav");

	auto scene = EndScene::createScene();
	Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));

}


void PicrossGameScene::goToWinScene(Ref *pSender){

	Global::LIFE = lifes;
	Global::TIME = tiempo_ms;

	//Guardar partida
	int saved_time = UserDefault::getInstance()->getIntegerForKey(("n_" + to_string(Global::PUZZLE_NUMBER) + "_tiempo").c_str());
	int saved_fallos = UserDefault::getInstance()->getIntegerForKey(("n_" + to_string(Global::PUZZLE_NUMBER) + "_fallos").c_str());
	bool completed = UserDefault::getInstance()->getBoolForKey(("n_" + to_string(Global::PUZZLE_NUMBER)).c_str());

	log("%d", saved_time);

	if (!completed)
	{
		UserDefault::getInstance()->setIntegerForKey(("n_" + to_string(Global::PUZZLE_NUMBER) + "_fallos").c_str(), 3 - lifes);
		if (Global::GAMEMODE == GameMode::NORMAL)
			UserDefault::getInstance()->setIntegerForKey(("n_" + to_string(Global::PUZZLE_NUMBER) + "_tiempo").c_str(), tiempo_ms);
		UserDefault::getInstance()->setBoolForKey(("n_" + to_string(Global::PUZZLE_NUMBER)).c_str(), true);
	}
	else
	{
		if (saved_fallos > 3 - lifes || saved_time > tiempo_ms)
			UserDefault::getInstance()->setIntegerForKey(("n_" + to_string(Global::PUZZLE_NUMBER) + "_fallos").c_str(), 3 - lifes);

		if (Global::GAMEMODE == GameMode::NORMAL && saved_time > tiempo_ms)
			UserDefault::getInstance()->setIntegerForKey(("n_" + to_string(Global::PUZZLE_NUMBER) + "_tiempo").c_str(), tiempo_ms);
	}
	UserDefault::getInstance()->flush();


	auto scene = WinScene::createScene();
	Director::getInstance()->pushScene(TransitionFade::create(0.5, scene));
}


void PicrossGameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_KP_MINUS)
	{
		if (picrossGridLayer->getScale() > 2)
		{
			picrossGridLayer->setScale(picrossGridLayer->getScale() - 0.1f);
			numbersLayer->setScale(numbersLayer->getScale() - 0.1f / initialScale);
			Global::PICROSS_SQUARE_SIDE = picrossGridVector[0][0]->getBoundingBox().size.width*picrossGridLayer->getScale();
		}
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_KP_PLUS)
	{
		if (picrossGridLayer->getScale() < 5)
		{
			picrossGridLayer->setScale(picrossGridLayer->getScale() + 0.1f);
			numbersLayer->setScale(numbersLayer->getScale() + 0.1f / initialScale);
			Global::PICROSS_SQUARE_SIDE = picrossGridVector[0][0]->getBoundingBox().size.width*picrossGridLayer->getScale();
		}
	}
}

void PicrossGameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	//log("Key with keycode %d released", keyCode);
}

bool PicrossGameScene::rightSquare(Picross* picross, vector<vector<int>> userSolution)
{
	for (int i = 0; i < (int)userSolution.size(); i++)
	{
		for (int j = 0; j < (int)userSolution[i].size(); j++)
		{
			if (userSolution[i][j] == 1)
				if (picross->getSolution()[i][j] != userSolution[i][j])
					return false;
		}
	}
	return true;
}

