#pragma once
#include "Card.hpp"
#include "Player.hpp"
#include <jansson.h>
#include "Position.hpp"

class Battlefield {
	std::optional<std::shared_ptr<Card>> TOP_LEFT;
	std::optional<std::shared_ptr<Card>> TOP_RIGHT;
	std::optional<std::shared_ptr<Card>> BOTTOM_LEFT;
	std::optional<std::shared_ptr<Card>> BOTTOM_RIGHT;
public:
	std::optional<std::shared_ptr<Card>>& findCard(int index);
	json_t* serialize();
};

class FieldSegment {
	std::array<std::optional<std::shared_ptr<Card>>, 4> cards;
public:
	std::optional<std::shared_ptr<Card>>& findCard(int index);
	json_t* serialize();
};

class PlayerField {
	Battlefield battlefield;
	FieldSegment standby;
	FieldSegment situation;
public:
	std::optional<std::shared_ptr<Card>>& findCard(FieldSegmentEnum fs, int index);
	json_t* serialize();
};

class PlayerIterator {
	PlayerField& pf;
	FieldSegmentEnum fs;
	int index;
	PlayerIterator(PlayerField& pf, FieldSegmentEnum fs, int index): pf(pf), fs(fs), index(index) {
	};
public:
	PlayerIterator(PlayerField& pf): pf(pf), fs(FieldSegmentEnum::STANDBY), index(-1) {
		++(*this);
	};
	PlayerIterator begin();
	PlayerIterator end();
	PlayerIterator& operator++();
	bool operator!=(PlayerIterator& other);
	std::shared_ptr<Card> operator*();
};

class Field {
private:
	PlayerField p1;
	PlayerField p2;

public:
	std::optional<std::shared_ptr<Card>>& findCard(Position pos);

	json_t* serialize();

	PlayerIterator iterateForPlayer(PlayerSide player);
};

