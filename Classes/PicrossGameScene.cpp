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

	//Se crea un Picross basado en los par�metros elegidos dirante las pantallas de selecci�n
	picross = new Picross(Constant::PUZZLE_NUMBER, Constant::GAMEMODE);
	//Test de n�mero de filas y columnas
	CCLOG("ROWS: %d, COLS: %d",picross->getRowCount(),picross->getColumnCount());

	return true;
}

