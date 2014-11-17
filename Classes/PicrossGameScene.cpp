#include "PicrossGameScene.h"
#include "Constant.h"

USING_NS_CC;

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
std::vector<Sprite*> PicrossGameScene::createSquareGrid()
{
	std::vector<Sprite*> grid = std::vector<Sprite*>(0);

	//TEMPORAL, hay que cambiarlo para que sea un vector de vectores
	for(int i = 0; i < picross->getColumnCount(); i++) //Columnas
	{
		for(int j = 0; j < picross->getRowCount(); j++) //Filas
		{
			Sprite* sprite = Sprite::create("picross_images/empty_selector.png");
			sprite->setScale(5);
			sprite->setPosition(500+sprite->getBoundingBox().size.width*i,500+sprite->getBoundingBox().size.height*-j);
			addChild(sprite,0);
			grid.push_back(sprite);
		}
	}

	log("VECTOR SIZE %d", grid.size());

	for(unsigned int i = 0; i < grid.size(); i++)
	{
		//Prueba
		log("TEST %f",grid[i]->getPosition().x);
	}

	return grid;
}

std::vector<Sprite*> PicrossGameScene::createTriangleGrid()
{
	std::vector<Sprite*> grid = std::vector<Sprite*>(0);

	return grid;
}