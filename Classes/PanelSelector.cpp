#include "PanelSelector.h"
#include <string>
#include <iostream>

USING_NS_CC;
using namespace std;

PanelSelector::PanelSelector(int num)
{
	readPanel(num);
	createGrid();
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

void PanelSelector::createGrid()
{


}
