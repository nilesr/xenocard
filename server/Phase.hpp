#pragma once
#include <string>

enum class Phase {
	P1_DRAW,
	P1_MOVE,
	P1_EVENT,
	P1_SET,
	P1_BLOCK,
	P1_BATTLE,
	P1_ADJUST,
	P2_DRAW,
	P2_MOVE,
	P2_EVENT,
	P2_SET,
	P2_BLOCK,
	P2_BATTLE,
	P2_ADJUST,
};

Phase phaseFromString(std::string phase);

Phase nextPhase(Phase phase);
