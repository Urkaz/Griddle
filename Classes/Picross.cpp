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
		mode = "f";

    string nombre = "";
    
    
	string fullPath = CCFileUtils::getInstance()->fullPathForFilename(mode+"_"+to_string(num)+".dat").c_str();

	FILE *stream = fopen(fullPath.c_str(),"r");
    
	if(stream == nullptr) perror(("Error opening file: /picross_data/"+mode+"_"+to_string(num)+".dat").c_str());
	else
	{
		short c = 0;

		// Los dos primeros numeros son las filas y columnas
		rows = fgetc(stream);
		columns = fgetc(stream);

		// Los dem�s la cuadr�cula
		while(c!= EOF)
		{
			c = fgetc(stream);
			if(c!= EOF)  //No se que es E0F, no me carga bien el nombre, revisalo ;)
			{
                
                matrixSolution = vector<vector<int>>(rows);
                for(int i = 0; i < rows; i++)
                {
                    vector<int> row = vector<int>(columns);
                    for(int j = 0; j < columns; j++)
                    {
                        row[j] = c;
                        log("%d",row[j]);
                    }
                    matrixSolution[i] = row;
                }
                
                log("%d",c);
			}
            //nombre = fgetc(stream);
            //log("%s - HOLA", nombre);
		}
        
	}
    
    
    
    
    
}

//Devuelve el n�mero de filas que tiene el Picross.
short Picross::getRowNumber()
{
	return rows;
}

//Devuelve el n�mero de columnas que tiene el Picross.
short Picross::getColumnNumber()
{
	return columns;
}

//Devuelve la matriz de soluci�n.
vector<vector<int>> Picross::getSolution()
{
	return solution;
}