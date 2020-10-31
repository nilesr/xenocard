#include "Player.hpp"
#include <iostream>

std::unique_ptr<Instruction> Player::recvInstruction() {
	while (true) {
		try {
			return this->conn.readInstruction();
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			this->conn.sendError(e.what());
		}
	}
}

void Player::sendError(std::string what) {
	this->conn.sendError(what);
}

