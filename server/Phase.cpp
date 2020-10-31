#include "Phase.hpp"
#include <map>
#include <stdexcept>

Phase phaseFromString(std::string phase) {
	static const std::map<std::string, Phase> phases {
		{"P1_DRAW", Phase::P1_DRAW},
		{"P1_MOVE", Phase::P1_MOVE},
		{"P1_EVENT", Phase::P1_EVENT},
		{"P1_SET", Phase::P1_SET},
		{"P1_BLOCK", Phase::P1_BLOCK},
		{"P1_BATTLE", Phase::P1_BATTLE},
		{"P1_ADJUST", Phase::P1_ADJUST},
		{"P2_DRAW", Phase::P2_DRAW},
		{"P2_MOVE", Phase::P2_MOVE},
		{"P2_EVENT", Phase::P2_EVENT},
		{"P2_SET", Phase::P2_SET},
		{"P2_BLOCK", Phase::P2_BLOCK},
		{"P2_BATTLE", Phase::P2_BATTLE},
		{"P2_ADJUST", Phase::P2_ADJUST},
	};
	auto iter = phases.find(phase);
	if (iter != phases.end()) return iter->second;
	throw std::logic_error{"Bad phase"};
}


Phase nextPhase(Phase phase) {
	switch (phase) {
		case Phase::P1_DRAW: return Phase::P1_MOVE;
		case Phase::P1_MOVE: return Phase::P1_EVENT;
		case Phase::P1_EVENT: return Phase::P1_SET;
		case Phase::P1_SET: return Phase::P1_BLOCK;
		case Phase::P1_BLOCK: return Phase::P1_BATTLE;
		case Phase::P1_BATTLE: return Phase::P1_ADJUST;
		case Phase::P1_ADJUST: return Phase::P2_DRAW;
		case Phase::P2_DRAW: return Phase::P2_MOVE;
		case Phase::P2_MOVE: return Phase::P2_EVENT;
		case Phase::P2_EVENT: return Phase::P2_SET;
		case Phase::P2_SET: return Phase::P2_BLOCK;
		case Phase::P2_BLOCK: return Phase::P2_BATTLE;
		case Phase::P2_BATTLE: return Phase::P2_ADJUST;
		case Phase::P2_ADJUST: return Phase::P1_DRAW;
	}
	throw std::logic_error{"should never get here"};
}

