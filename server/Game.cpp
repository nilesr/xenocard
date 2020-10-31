#include "Game.hpp"
#include <iostream>

void Game::run() {
	while (true) {
		auto& plr = (
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
			case InstructionType::SET_PHASE:
				{
					auto spi = dynamic_cast<SetPhaseInstruction*>(instruction.get());
					std::cout << "Would-be new phase: " << (int)spi->newPhase << std::endl;
					if (spi->newPhase == nextPhase(this->phase)) {
						this->phase = spi->newPhase;
						std::cout << "New phase: " << (int)this->phase << std::endl;
					}
				}
				break;
			default:
				plr.sendError("Unrecognized instruction format");
				break;
		}
		// TODO actually inform both parties of the new state
		this->p1.sendError("Phase was updated");
		this->p2.sendError("Phase was updated");
	}
}
