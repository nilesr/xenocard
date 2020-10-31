#pragma once
#include "Phase.hpp"
#include "Position.hpp"

enum class InstructionType {
	SHUFFLE,
	SET_PHASE,
	PLAY,
	MOVE,
	USE_CARD_EFFECT,
	DISCARD,
};

struct Instruction {
	virtual InstructionType getType() = 0;
};

struct SetPhaseInstruction: public Instruction {
	InstructionType getType() override { return InstructionType::SET_PHASE; };

	SetPhaseInstruction(Phase newPhase): newPhase(newPhase) {};
	Phase newPhase;
};

struct ShuffleInstruction: public Instruction {
	InstructionType getType() override { return InstructionType::SHUFFLE; };
};

struct PlayInstruction: public Instruction {
	InstructionType getType() override { return InstructionType::PLAY; };

	PlayInstruction(int handIndex, Position position): handIndex(handIndex), position(position) {};
	int handIndex;
	Position position;
};

struct DiscardInstruction: public Instruction {
	InstructionType getType() override { return InstructionType::DISCARD; };

	DiscardInstruction(int handIndex): handIndex(handIndex) {};
	int handIndex;
};


struct MoveInstruction: public Instruction {
	InstructionType getType() override { return InstructionType::MOVE; };

	MoveInstruction(Position start, Position end): start(start), end(end) {};
	Position start;
	Position end;
};

