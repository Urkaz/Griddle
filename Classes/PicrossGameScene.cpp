#include "PicrossGameScene.h"
#include "Constant.h"

using namespace cocos2d;
using namespace std;

short Constant::PICROSS_SQUARE_SIDE;

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

	//Se crea un Picross basado en los parámetros elegidos durante las pantallas de selección
	picross = new Picross(Constant::PUZZLE_NUMBER, Constant::GAMEMODE);

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
	picrossGridLayer->setPosition(
		picrossGridVector[0].size()/2*Constant::PICROSS_SQUARE_SIDE+picrossGridVector[0].size()%2*Constant::PICROSS_SQUARE_SIDE/2,
		picrossGridVector.size()/2*Constant::PICROSS_SQUARE_SIDE+picrossGridVector.size()%2*Constant::PICROSS_SQUARE_SIDE/2);

	//Se dibuja el tablero.
	addChild(picrossGridLayer);

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

			//Se signa la posición al sprite relativa a la cuadrícula
			int spriteOffsetX = sprite->getBoundingBox().size.width/2-sprite->getBoundingBox().size.width*picross->getColumnNumber()/2;
			int spriteOffsetY = -sprite->getBoundingBox().size.height/2+sprite->getBoundingBox().size.height*picross->getRowNumber()/2;

			int posX = spriteOffsetX+sprite->getBoundingBox().size.width*j;
			int posY = spriteOffsetY+sprite->getBoundingBox().size.height*-i;

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
	//Se obtiene el ancho y alto de la zona visible
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int layerOffsetX = visibleSize.width  /2;
	int layerOffsetY = visibleSize.height /2;

	//Se crea una capa en el centro de la pantalla a la que se le añadiran los sprites
	Layer* spriteLayer = Layer::create();
	spriteLayer->setContentSize(Size(1, 1));
	spriteLayer->setPosition(Vec2(layerOffsetX, layerOffsetY));
	spriteLayer->setScale(5);

	for(unsigned int i = 0; i < spriteVector.size(); i++) //Fila
		for(unsigned int j = 0; j < spriteVector[i].size(); j++) //Columna
			spriteLayer->addChild(spriteVector[i][j],0);

	return spriteLayer;
}

void PicrossGameScene::onMouseDown(Event* event)
{
	auto *e = (EventMouse*)event;


	short i = (int)e->getCursorY()/Constant::PICROSS_SQUARE_SIDE; //"i" representa el número de fila
	short j = (int)e->getCursorX()/Constant::PICROSS_SQUARE_SIDE; //"j" representa el número de columna dentro de la fila "i"

	i = std::abs(i-picross->getRowNumber()+1);

	log("ROW:(i): %d , COL(j) %d",i,j);

	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("picross_images/markX_picross.png");

	if(i < picross->getRowNumber() && j < picross->getColumnNumber())
		picrossGridVector[i][j]->setTexture(texture);
}