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
		picrossGridVector = createSquareGrid();
	else //NYI
		picrossGridVector = createTriangleGrid();

	return true;
}

//Devuelve la matriz de Sprites que forman un Picross cuadrado y los añade a la escena.
vector<vector<Sprite*>> PicrossGameScene::createSquareGrid()
{
	vector<vector<Sprite*>> grid = vector<vector<Sprite*>>(picross->getColumnCount());

	Size visibleSize = Director::getInstance()->getVisibleSize();
	int layerOffsetX = visibleSize.width  /2;
	int layerOffsetY = visibleSize.height /2;

	//picrossGridS = Layer::create();
	Layer* spriteLayer = Layer::create();
	spriteLayer->setContentSize(Size(1, 1));
	spriteLayer->setPosition(Vec2(layerOffsetX, layerOffsetY));
	spriteLayer->setScale(5);

	for(int i = 0; i < picross->getColumnCount(); i++) //Columnas
	{
		vector<Sprite*> row = vector<Sprite*>(picross->getRowCount());
		for(int j = 0; j < picross->getRowCount(); j++) //Filas
		{
			Sprite* sprite = Sprite::create("picross_images/empty_picross.png");

			//Prueba cambio de textura
			Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("picross_images/markX_picross.png");
			if((j+i)%2==0) sprite->setTexture(texture);


			int spriteOffsetX = sprite->getBoundingBox().size.width/2-sprite->getBoundingBox().size.width*picross->getColumnCount()/2;
			int spriteOffsetY = -sprite->getBoundingBox().size.height/2+sprite->getBoundingBox().size.height*picross->getRowCount()/2;

			sprite->setPosition(spriteOffsetX+sprite->getBoundingBox().size.width*i,
								spriteOffsetY+sprite->getBoundingBox().size.height*-j);

			spriteLayer->addChild(sprite,0);
			row[j] = sprite;
		}
		grid[i] = row;
	}

	addChild(spriteLayer);

	/*for(unsigned int i = 0; i < grid.size(); i++)
	{
		//Prueba
		log("TEST %f",grid[i][0]->getPosition().x);
	}*/

	return grid;
}

//Devuelve la matriz de Sprites que forman un Picross triangular y los añade a la escena.
vector<vector<Sprite*>> PicrossGameScene::createTriangleGrid()
{
	vector<vector<Sprite*>> grid = vector<vector<Sprite*>>(0);

	return grid;
}

void PicrossGameScene::onMouseDown(Event* event)
{
	auto *e = dynamic_cast<EventMouse *>(event);

	log("%f-%f",e->getCursorX(),e->getCursorY());
	//labelTouchInfo->setString("You Touched Here");
}