#include "PicrossGameScene.h"
#include "Constant.h"

using namespace cocos2d;
using namespace std;

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

	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(PicrossGameScene::onMouseDown, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

	//Se crea un Picross basado en los parámetros elegidos durante las pantallas de selección
	picross = new Picross(Constant::PUZZLE_NUMBER, Constant::GAMEMODE);

	//Se comprueba el GameMode
	if(Constant::GAMEMODE != GameMode::TRIANGLES)
	{
		picrossGridVector = createSquareMatrix(picross);
		picrossGridLayer = createLayer(picrossGridVector);
	}
	//NO IMPLEMENTADO
	//else
		//picrossGridVector = createTriangleGrid();

	addChild(picrossGridLayer);
	return true;
}

/**
 * Crea la matriz de Sprites que forman un Picross cuadrado.
 *
 * vector[0] = fila 0;
 * vector[0][0] = fila 0, columna 0.
 * vector[1][2] = fila 1, columna 2.
 *
 * @param   picross	Instancia de la clase Picross.
 * @return  Vector de filas de Sprites del Picross.
 */
vector<vector<Sprite*>> PicrossGameScene::createSquareMatrix(Picross* picross)
{
	vector<vector<Sprite*>> matrix = vector<vector<Sprite*>>(picross->getColumnNumber());

	for(int i = 0; i < picross->getColumnNumber(); i++) //"i" representa el número de fila
	{
		//Se crea una nueva fila a la que añadir sprites
		vector<Sprite*> row = vector<Sprite*>(picross->getRowNumber());
		for(int j = 0; j < picross->getRowNumber(); j++) //"j" representa el número de columna dentro de la fila "i"
		{
			//Se crea un sprite con la imagen por defecto de vacío.
			Sprite* sprite = Sprite::create("picross_images/empty_picross.png");

			//Prueba cambio de textura
			Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("picross_images/markX_picross.png");
			if((j+i)%2==0) sprite->setTexture(texture);

			//Se signa la posición al sprite relativa a la cuadrícula
			int spriteOffsetX = sprite->getBoundingBox().size.width/2-sprite->getBoundingBox().size.width*picross->getColumnCount()/2;
			int spriteOffsetY = -sprite->getBoundingBox().size.height/2+sprite->getBoundingBox().size.height*picross->getRowCount()/2;

			sprite->setPosition(spriteOffsetX+sprite->getBoundingBox().size.width*i,
								spriteOffsetY+sprite->getBoundingBox().size.height*-j);

			//Se añade a la fila
			row[j] = sprite;
		}
		//La fila entera se añade a la matriz
		matrix[i] = row;
	}

	/*for(unsigned int i = 0; i < grid.size(); i++)
	{
		//Prueba
		log("TEST %f",grid[i][0]->getPosition().x);
	}*/

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
	auto *e = dynamic_cast<EventMouse *>(event);

	log("%f-%f",e->getCursorX(),e->getCursorY());
	//labelTouchInfo->setString("You Touched Here");
}