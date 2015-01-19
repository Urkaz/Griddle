#pragma once

#include "Picross.h"

class Global
{
public:
	static GameMode GAMEMODE;
	static short PUZZLE_NUMBER;
	
	static short PICROSS_SQUARE_SIDE;
	static short SELECTOR_SQUARE_SIDE;

	static short FONT_SIZE;

	static const short MAX_PACK_INDEX = 3;
	static short CURRENT_PACK_INDEX;

	static const short TIME_LIMIT = 5;

	static bool DEBUG;

	static short LIFE;
	static short TIME;
};