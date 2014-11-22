#include "PicrossGameScene.h"
#include "Constant.h"

using namespace cocos2d;
using namespace std;

short Constant::PICROSS_SQUARE_SIDE;

Texture2D* texMarkX;
Texture2D* texDraw;
Texture2D* texEmpty;
Texture2D* texButtonDraw;
Texture2D* texButtonMarkX;
Texture2D* texButtonClickDraw;
Texture2D* texButtonClickMarkX;

Texture2D::TexParams textureParams;

Sprite* button_draw;
Sprite* button_X;

bool drawEnabled = false;
bool markXEnabled = false;

Size visibleSize;

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

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

	//Carga de texturas
	Texture2D::TexParams tp = {GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
	textureParams = tp;
	texMarkX = Director::getInstance()->getTextureCache()->addImage("picross_images/markX_picross.png");
	texDraw = Director::getInstance()->getTextureCache()->addImage("picross_images/draw_picross.png");
	texEmpty = Director::getInstance()->getTextureCache()->addImage("picross_images/empty_picross.png");

	texButtonDraw = Director::getInstance()->getTextureCache()->addImage("picross_images/boton_lapiz.png");
	texButtonMarkX = Director::getInstance()->getTextureCache()->addImage("picross_images/boton_X.png");
	texButtonClickDraw = Director::getInstance()->getTextureCache()->addImage("picross_images/boton_click_lapiz.png");
	texButtonClickMarkX = Director::getInstance()->getTextureCache()->addImage("picross_images/boton_click_X.png");

	texMarkX->setTexParameters(textureParams);
	texDraw->setTexParameters(textureParams);
	texEmpty->setTexParameters(textureParams);

	//Se crea un Picross basado en los parámetros elegidos durante las pantallas de selección
	picross = new Picross(Constant::PUZZLE_NUMBER, Constant::GAMEMODE);

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
	if(Constant::GAMEMODE != GameMode::TRIANGLES)
	{
		picrossGridVector = createSquareMatrix(picross);
		picrossGridLayer = createLayer(picrossGridVector);
	}
	//NO IMPLEMENTADO
	/*else
		picrossGridVector = createTriangleGrid();*/

	//Se define la constante del lado de cada cuadrado del tablero.
	Constant::PICROSS_SQUARE_SIDE = picrossGridVector[0][0]->getBoundingBox().size.width*picrossGridLayer->getScale();

	//TEMPORAL: Se recolocal el tablero al (0,0) para comprobar las coordenadas.
	/*picrossGridLayer->setPosition(
		picrossGridVector[0].size()/2*Constant::PICROSS_SQUARE_SIDE+picrossGridVector[0].size()%2*Constant::PICROSS_SQUARE_SIDE/2+picrossGridLayer->getBoundingBox().size.width/2,
		picrossGridVector.size()/2*Constant::PICROSS_SQUARE_SIDE+picrossGridVector.size()%2*Constant::PICROSS_SQUARE_SIDE/2+picrossGridLayer->getBoundingBox().size.height/2);
		*/

	Size visibleSize = Director::getInstance()->getVisibleSize();

	button_draw = Sprite::create("picross_images/boton_lapiz.png");
	button_X = Sprite::create("picross_images/boton_X.png");
	button_draw->setPosition(button_draw->getBoundingBox().size.width/2,visibleSize.height/2+button_draw->getBoundingBox().size.height/2);
	button_X->setPosition(button_X->getBoundingBox().size.width/2,button_X->getBoundingBox().size.height/2);

	//Se dibuja el tablero.
	addChild(picrossGridLayer);
	addChild(button_draw);
	addChild(button_X);

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
			Sprite* sprite = Sprite::create("picross_images/empty_picross.png");
			sprite->getTexture()->setTexParameters(textureParams);

			//Se signa la posición al sprite relativa a la cuadrícula
			float spriteOffsetX = sprite->getBoundingBox().size.width/2-sprite->getBoundingBox().size.width*picross->getColumnNumber()/2;
			float spriteOffsetY = -sprite->getBoundingBox().size.height/2+sprite->getBoundingBox().size.height*picross->getRowNumber()/2;

			float posX = spriteOffsetX+sprite->getBoundingBox().size.width*j;
			float posY = spriteOffsetY+sprite->getBoundingBox().size.height*-i;

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
Layer* PicrossGameScene::createLayer(vector<vector<Sprite*>> spriteVector)
{
	//Se crea una capa en el centro de la pantalla a la que se le añadiran los sprites
	Layer* spriteLayer = Layer::create();
	spriteLayer->setContentSize(Size(1, 1));
	spriteLayer->setScale(5);

	//Se establece la posición
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int layerOffsetX = visibleSize.width  /2 + spriteLayer->getBoundingBox().size.width/2;
	int layerOffsetY = visibleSize.height /2 + spriteLayer->getBoundingBox().size.height/2;

	spriteLayer->setPosition(Vec2(layerOffsetX, layerOffsetY));

	for(unsigned int i = 0; i < spriteVector.size(); i++) //Fila
		for(unsigned int j = 0; j < spriteVector[i].size(); j++) //Columna
			spriteLayer->addChild(spriteVector[i][j],0);

	return spriteLayer;
}

void PicrossGameScene::onMouseDown(Event* event)
{
	auto *e = (EventMouse*)event;
	float cursorY = e->getCursorY();
	float cursorX = e->getCursorX();

	//Offset del tablero respecto al 0,0
	int offSetX = picrossGridLayer->getPosition().x - picrossGridVector[0].size()/2 * Constant::PICROSS_SQUARE_SIDE + picrossGridVector[0].size()%2 * Constant::PICROSS_SQUARE_SIDE/2 - picrossGridLayer->getBoundingBox().size.height/2;
	int offSetY = picrossGridLayer->getPosition().y - picrossGridVector.size()/2 * Constant::PICROSS_SQUARE_SIDE - picrossGridVector.size()%2 * Constant::PICROSS_SQUARE_SIDE/2 - picrossGridLayer->getBoundingBox().size.width/2;

	//Coordenadas fila(i),columna(j)
	short i = (cursorY-offSetY)/Constant::PICROSS_SQUARE_SIDE;
	short j = (cursorX-offSetX)/Constant::PICROSS_SQUARE_SIDE;

	i = std::abs(i-picross->getRowNumber()+1);

	//log("ROW:(i): %d , COL(j) %d",i,j);

	Size visibleSize = Director::getInstance()->getVisibleSize();

	if(cursorX < button_draw->getBoundingBox().size.width)
	{
		if(cursorY < visibleSize.height/2)
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
		else
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
		cursorY < offSetY+Constant::PICROSS_SQUARE_SIDE*picrossGridVector.size() &&
		cursorX < offSetX+Constant::PICROSS_SQUARE_SIDE*picrossGridVector[0].size())
	{
		//Pintar
		if(drawEnabled && !markXEnabled)
		{
			switch(userSolution[i][j])
			{
			case -1: //X
				userSolution[i][j] = 0;
				picrossGridVector[i][j]->setTexture(texEmpty);
				break;
			case 0: //Vacio
				userSolution[i][j] = 1;
				picrossGridVector[i][j]->setTexture(texDraw);
				break;
			case 1: //Pintado
				userSolution[i][j] = 0;
				picrossGridVector[i][j]->setTexture(texEmpty);
				break;
			}
		}
		//Marcar X
		else if(!drawEnabled && markXEnabled)
		{
			switch(userSolution[i][j])
			{
			case -1: //X
				userSolution[i][j] = 0;
				picrossGridVector[i][j]->setTexture(texEmpty);
				break;
			case 0: //Vacio
				userSolution[i][j] = -1;
				picrossGridVector[i][j]->setTexture(texMarkX);
				break;
			case 1: //Pintado
				userSolution[i][j] = 0;
				picrossGridVector[i][j]->setTexture(texEmpty);
				break;
			}
		}
		//ELSE (ninguna de las dos cosas marcadas)
				//MOVER EL TABLERO
	}
}