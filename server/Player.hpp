#pragma once
#include "Connection.hpp"
#include "Instruction.hpp"
#include "Card.hpp"
#include "Random.hpp"
#include "PlayerSide.hpp"
#include <memory>
#include <vector>

#include "cards/Realian.hpp"
#include "cards/Gun.hpp"
#include "cards/Human.hpp"
#include "cards/Mech.hpp"
#include "cards/Curry.hpp"

class Player {
	const PlayerSide which;
	Connection conn;
	Random& random;

public:
	Player(PlayerSide which, Connection conn, Random& random): which(which), conn(conn), random(random) {
		// TEMP
		for (int i = 0; i < 3; i++) deck.push_back(std::make_shared<Realian>());
		for (int i = 0; i < 3; i++) deck.push_back(std::make_shared<Gun>());
		for (int i = 0; i < 3; i++) deck.push_back(std::make_shared<Human>());
		for (int i = 0; i < 3; i++) deck.push_back(std::make_shared<Mech>());
		for (int i = 0; i < 3; i++) deck.push_back(std::make_shared<Curry>());
	};

	std::vector<std::shared_ptr<Card>> hand;
	std::vector<std::shared_ptr<Card>> deck;
	std::vector<std::shared_ptr<Card>> junk;
	std::vector<std::shared_ptr<Card>> lost;

	std::unique_ptr<Instruction> recvInstruction();
	void sendError(std::string what);

	void dealInitialHand();
	void sendState(SerializedGame game);
	void drawCard();
	void payCost(int i);
};
