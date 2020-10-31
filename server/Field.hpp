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

class Field {
private:
	PlayerField p1;
	PlayerField p2;

public:
	std::optional<std::shared_ptr<Card>>& findCard(Position pos);

	json_t* serialize();

};
