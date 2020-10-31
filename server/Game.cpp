#include "Game.hpp"
#include <iostream>

void Game::run() {
	this->p1.dealInitialHand();
	this->p2.dealInitialHand();
	auto shuffles = 0;
	while (true) {
		this->sendState();
		auto& plr = (
			this->phase == Phase::P1_SHUFFLE ||
			this->phase == Phase::P1_DRAW ||
			this->phase == Phase::P1_MOVE ||
			this->phase == Phase::P1_EVENT ||
			this->phase == Phase::P1_SET ||
			this->phase == Phase::P2_BLOCK ||
			this->phase == Phase::P1_BATTLE ||
			this->phase == Phase::P1_ADJUST
		) ? this->p1 : this->p2;
		auto instruction = plr.recvInstruction();
		switch (instruction->getType()) {
			case InstructionType::SHUFFLE:
				{
					if ((this->phase == Phase::P1_SHUFFLE || this->phase == Phase::P2_SHUFFLE) && shuffles < 3) {
						plr.dealInitialHand();
						shuffles++;
					} else {
						plr.sendError("You may not shuffle at this time, either it is not that phase, or you have already shuffled 3 times");
					}
				}
				break;
			case InstructionType::SET_PHASE:
				{
					auto spi = dynamic_cast<SetPhaseInstruction*>(instruction.get());
					std::cout << "Would-be new phase: " << (int)spi->newPhase << std::endl;
					if (spi->newPhase == nextPhase(this->phase)) {
						this->phase = spi->newPhase;
						std::cout << "New phase: " << (int)this->phase << std::endl;
						if (this->phase == Phase::P2_SHUFFLE) {
							shuffles = 0;
						}
						if (this->phase == Phase::P1_DRAW) {
							this->turn++;
						}
					} else {
						plr.sendError("You cannot set that phase right now");
					}
				}
				break;
			default:
				plr.sendError("Unrecognized instruction type in run_game");
				break;
		}
	}
}

void Game::sendState() {
	auto p1game = this->serializeForPlayer(PlayerSide::P1);
	p1.sendState(p1game);
	auto p2game = this->serializeForPlayer(PlayerSide::P2);
	p2.sendState(p2game);
}

SerializedGame Game::serializeForPlayer(PlayerSide player) {
	auto& plr = player == PlayerSide::P1 ? this->p1 : this->p2;
	auto& enemy = player == PlayerSide::P1 ? this->p2 : this->p1;
	return SerializedGame{
		player == PlayerSide::P1 ? "P1" : "P2",
		this->turn,
		phaseToString(this->phase),
		this->field.serialize(),
		serializeCards(plr.hand),
		(int) plr.deck.size(),
		(int) enemy.hand.size(),
		(int) enemy.deck.size(),
	};
}

