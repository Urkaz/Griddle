#include <iostream>
#include "Picross.h"
#include <string>

USING_NS_CC;
using namespace std;

Picross::Picross(short num, GameMode gm)
{
	string mode = "";
	if(gm == GameMode::NORMAL)
		mode = "n";
	else if(gm == GameMode::FREE)
		//mode = "f";
		mode = "n";
    
	string fullPath = CCFileUtils::getInstance()->fullPathForFilename(mode+"_"+to_string(num)+".dat");

	FILE *stream = fopen(fullPath.c_str(),"r");
    
	if(stream == nullptr) perror(("Error opening file: /picross_data/"+mode+"_"+to_string(num)+".dat").c_str());
	else
	{
		short c = 0;

		// Los dos primeros numeros son las filas y columnas
		rows = fgetc(stream);
		columns = fgetc(stream);

		//Leer matriz
		matrixSolution = vector<vector<int>>(rows);
		for(int i = 0; i < rows; i++)
		{
			vector<int> row = vector<int>(columns);
			for(int j = 0; j < columns; j++)
			{
				c = fgetc(stream);
				row[j] = c;
				//log("Fila %d - %d",j,row[j]);
			}
			matrixSolution[i] = row;
		}
        
		//Leer nombre
		name = "";
		while(c != 0xFF)
		{
			c = fgetc(stream);
			if(c != 0xFF)
				name = name + (char)c;
		}

		log("NOMBRE: %s", name.c_str());

		//Leer autor
		author = "";
		while (c != EOF)
		{
			c = fgetc(stream);
			if (c != EOF)
				author = author + (char)c;
		}

		log("AUTOR: %s", author.c_str());
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

//Devuelve la matriz de solución.
vector<vector<int>> Picross::getSolution()
{
	return matrixSolution;
}