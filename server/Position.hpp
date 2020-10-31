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

Position positionFromJson(json_t* obj);
