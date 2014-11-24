#include <iostream>
#include "Picross.h"

USING_NS_CC;
using namespace std;

Picross::Picross(short num, GameMode gm)
{
	string mode = "";
	if(gm == GameMode::NORMAL)
		mode = "n";
	else if(gm == GameMode::FREE)
		mode = "f";


	string fullPath = CCFileUtils::getInstance()->fullPathForFilename(to_string(num)+".dat").c_str();

	FILE *stream = fopen(fullPath.c_str(),"r");

	if(stream == nullptr) perror(("Error opening file: /picross_data/"+mode+"/"+to_string(num)+".dat").c_str());
	else
	{
		short c = 0;

		// Los dos primeros numeros son las filas y columnas
		rows = fgetc(stream);
		columns = fgetc(stream);

		// Los demás la cuadrícula
		while(c!= EOF)
		{
			c = fgetc(stream);
			if(c!= EOF)
			{
				//log("%d",c);
			}
		}
	}
}

//Devuelve el número de filas que tiene el Picross.
short Picross::getRowNumber()
{
	return rows;
}

//Devuelve el número de columnas que tiene el Picross.
short Picross::getColumnNumber()
{
	return columns;
}