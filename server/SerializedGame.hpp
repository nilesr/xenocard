#pragma once
#include <jansson.h>

struct SerializedGame {
	std::string player;
	int turn;
	std::string phase;
	json_t* field;
	json_t* hand;
	json_t* junk;
	int deckSize;
	int lostSize;
	int enemyHandSize;
	int enemyDeckSize;
	int enemyJunkSize;
	int enemyLostSize;
};

