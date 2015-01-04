#include "PanelSelector.h"
#include <string>
#include <iostream>
#include "Constant.h"

USING_NS_CC;
using namespace std;

Texture2D::TexParams texParams;
short Constant::SELECTOR_SQUARE_SIDE;

PanelSelector::PanelSelector(int num)
{
	texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };

	setFirst = false;

	packID = num;
	readPanel(packID);
	createLayer();
}

void PanelSelector::readPanel(int num)
{
	string fullPath = CCFileUtils::getInstance()->fullPathForFilename("pack_" + to_string(num) + ".dat");

	log("%s", fullPath.c_str());

	FILE *stream = fopen(fullPath.c_str(), "r");

	if (stream == nullptr) perror(("Error opening file: /picross_data/pack_" + to_string(num) + ".dat").c_str());
	else
	{
		short c = 0;

		//Leer matriz
		picrossID = vector<vector<int>>(5);
		for (int i = 0; i < 5; i++)
		{
			vector<int> row = vector<int>(5);
			for (int j = 0; j < 5; j++)
			{
				c = fgetc(stream);
				row[j] = c;
				//log("Fila %d - %d",j,row[j]);
			}
			picrossID[i] = row;
		}

		//Leer nombre
		name = "";
		while (c != EOF)
		{
			c = fgetc(stream);
			if (c != EOF)
				name = name + (char)c;
		}

		//log("NOMBRE: %s", name.c_str());
	}
}

void PanelSelector::createLayer()
{
	Layer* layer = Layer::create();
	layer->setContentSize(Size(1, 1));

	vector<vector<Sprite*>> matrix = vector<vector<Sprite*>>(5);

	for (int i = 0; i < 5; i++) //"i" representa el número de fila
	{
		//Se crea una nueva fila a la que añadir sprites
		vector<Sprite*> row = vector<Sprite*>(5);
		for (int j = 0; j < 5; j++) //"j" representa el número de columna dentro de la fila "i"
		{
			Sprite* sprite = Sprite::create("empty_selector.png");
			if (picrossID[i][j] != 0)
			{
				if (!setFirst)
				{
					firstPicrossSpriteIndexRow = i;
					firstPicrossSpriteIndexCol = j;
					setFirst = true;
				}
				//Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("n_" + to_string(picrossID[i][j]) + ".png");
				Texture2D* texture;

				bool completed = UserDefault::getInstance()->getBoolForKey(("n_" + to_string(picrossID[i][j])).c_str());
				int tiempo = UserDefault::getInstance()->getIntegerForKey(("n_" + to_string(picrossID[i][j])+"_tiempo").c_str());
				//log("Completed %s", completed ? "true" : "false");

				if (!completed)
					texture = Director::getInstance()->getTextureCache()->addImage("unknown.png");
				else
				{
					if (tiempo < Constant::TIME_LIMIT*60)
						texture = Director::getInstance()->getTextureCache()->addImage("n_" + to_string(picrossID[i][j]) + ".png");
					else
						texture = Director::getInstance()->getTextureCache()->addImage("n_" + to_string(picrossID[i][j]) + "_u.png");
				}

				sprite->setTexture(texture);

			}
			sprite->getTexture()->setTexParameters(texParams);

			//Se signa la posición al sprite relativa a la cuadrícula
			float spriteOffsetX = sprite->getBoundingBox().size.width / 2 - sprite->getBoundingBox().size.width*5 / 2;
			float spriteOffsetY = -sprite->getBoundingBox().size.height / 2 + sprite->getBoundingBox().size.height*5 / 2;

			float posX = spriteOffsetX + sprite->getBoundingBox().size.width*j;
			float posY = spriteOffsetY + sprite->getBoundingBox().size.height*-i;

			sprite->setPosition(posX, posY);

			//Se añade a la fila
			row[j] = sprite;
			layer->addChild(sprite, 0);
		}
		//La fila entera se añade a la matriz
		matrix[i] = row;
	}
	gridVector = matrix;
	gridLayer = layer;

	Constant::SELECTOR_SQUARE_SIDE = gridVector[0][0]->getBoundingBox().size.width;
}

Layer* PanelSelector::getLayer()
{
	return gridLayer;
}

int PanelSelector::getPicrossID(int row, int col)
{
	return picrossID[row][col];
}

string PanelSelector::getPanelName()
{
	return name;
}

Texture2D* PanelSelector::getFirstPicrossTexture()
{
	return gridVector[firstPicrossSpriteIndexRow][firstPicrossSpriteIndexCol]->getTexture();
}

int PanelSelector::getFirstPicrossID()
{
	return picrossID[firstPicrossSpriteIndexRow][firstPicrossSpriteIndexCol];
}

Texture2D* PanelSelector::getPicrossTextureIndex(int i, int j)
{
	return gridVector[i][j]->getTexture();
}