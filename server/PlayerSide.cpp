#include "PlayerSide.hpp"

std::string playerSideToString(PlayerSide player) {
	return player == PlayerSide::P1 ? "P1" : "P2";
}

