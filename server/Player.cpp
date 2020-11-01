#include "Player.hpp"
#include <iostream>
#include <algorithm>

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

void Player::notify(json_t* object) {
	this->conn.notify(object);
}


void Player::dealInitialHand() {
	this->deck.insert(this->deck.end(), this->hand.begin(), this->hand.end());
	this->hand.clear();
	std::shuffle(this->deck.begin(), this->deck.end(), this->random);
	this->hand.insert(this->hand.begin(), this->deck.rbegin(), this->deck.rbegin() + 6);
	for (int i = 0; i < 6; i++) {
		this->deck.pop_back();
	}
}

void Player::sendState(SerializedGame game) {
	this->conn.sendGame(game);
}

void Player::drawCard() {
	this->hand.push_back(this->deck.back());
	this->deck.pop_back();
}

void Player::payCost(int n) {
	this->lost.insert(this->lost.end(), this->deck.rbegin(), this->deck.rbegin() + n);
	for (int i = 0; i < n; i++) {
		// TODO check if the deck is empty
		this->deck.pop_back();
	}
}



