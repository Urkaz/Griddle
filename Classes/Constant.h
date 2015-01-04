#pragma once

#include "Picross.h"

class Constant
{
public:
	static GameMode GAMEMODE;
	static short PUZZLE_NUMBER;

	static short PICROSS_SQUARE_SIDE;
	static short SELECTOR_SQUARE_SIDE;

	static short FONT_SIZE;

	static const short MAX_PACK_INDEX = 2;
	static short CURRENT_PACK_INDEX;

	static const short TIME_LIMIT = 5;
};

