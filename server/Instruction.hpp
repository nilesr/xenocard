#pragma once
#include "Phase.hpp"

enum class InstructionType {
	SET_PHASE,
	PLAY,
	MOVE,
	USE_CARD_EFFECT,
};

struct Instruction {
	virtual InstructionType getType() = 0;
};

struct SetPhaseInstruction: public Instruction {
	InstructionType getType() override { return InstructionType::SET_PHASE; };

	SetPhaseInstruction(Phase newPhase): newPhase(newPhase) {};
	Phase newPhase;
};

