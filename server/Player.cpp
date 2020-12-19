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

bool Player::payCost(int n) {
	for (int i = 0; i < n; i++) {
		if (this->deck.size() == 0) {
			return true;
		}
		this->lost.push_back(*this->deck.rbegin());
		this->deck.pop_back();
	}
	return false;
}

bool Player::isDead() {
	return this->deck.size() == 0;
}
