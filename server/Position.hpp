#pragma once
#include "PlayerSide.hpp"
#include <jansson.h>

enum class FieldSegmentEnum {
	STANDBY,
	BATTLEFIELD,
	SITUATION,
};

struct Position {
	PlayerSide playerSide;
	FieldSegmentEnum segment;
	int index;
};

PlayerSide playerSideFromString(std::string in);
FieldSegmentEnum fieldSegmentFromString(std::string in);
std::string fieldSegmentToString(FieldSegmentEnum in);

Position positionFromJson(const json_t* const obj);
json_t* serializePosition(Position p);
