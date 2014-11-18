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

	//Se crea un Picross basado en los parámetros elegidos durante las pantallas de selección
	picross = new Picross(Constant::PUZZLE_NUMBER, Constant::GAMEMODE);

	//Se comprueba el GameMode
	if(Constant::GAMEMODE != GameMode::TRIANGLES)
		picrossGrid = createSquareGrid();
	else //NYI
		picrossGrid = createTriangleGrid();

	return true;
}

//Devuelve la matriz de Sprites que forman un Picross cuadrado y los añade a la escena.
vector<vector<Sprite*>> PicrossGameScene::createSquareGrid()
{
	vector<vector<Sprite*>> grid = vector<vector<Sprite*>>(picross->getColumnCount());

	int offsetX = 500;
	int offsetY = 500;

	for(int i = 0; i < picross->getColumnCount(); i++) //Columnas
	{
		vector<Sprite*> row = vector<Sprite*>(picross->getRowCount());
		for(int j = 0; j < picross->getRowCount(); j++) //Filas
		{
			Sprite* sprite = Sprite::create("picross_images/empty_selector.png");
			sprite->setScale(5);
			sprite->setPosition(offsetX+sprite->getBoundingBox().size.width*i,offsetY+sprite->getBoundingBox().size.height*-j);
			addChild(sprite,0);
			row[j] = sprite;
		}
		grid[i] = row;
	}

	log("VECTOR SIZE %d", grid.size());

	for(unsigned int i = 0; i < grid.size(); i++)
	{
		//Prueba
		log("TEST %f",grid[i][0]->getPosition().x);
	}

	return grid;
}

//Devuelve la matriz de Sprites que forman un Picross triangular y los añade a la escena.
vector<vector<Sprite*>> PicrossGameScene::createTriangleGrid()
{
	vector<vector<Sprite*>> grid = vector<vector<Sprite*>>(0);

	return grid;
}