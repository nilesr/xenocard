#pragma once
#include <jansson.h>

struct SerializedGame {
	std::string player;
	int turn;
	std::string phase;
	json_t* field;
	json_t* hand;
	int deckSize;
	int enemyHandSize;
	int enemyDeckSize;
};

