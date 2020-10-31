#pragma once
#include "PlayerSide.hpp"
#include <jansson.h>

enum class FieldSegment {
	BATTLEFIELD,
	STANDBY,
	SITUATION,
};

struct Position {
	PlayerSide playerSide;
	FieldSegment segment;
	int index;
};

Position positionFromJson(json_t* obj);
