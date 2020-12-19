#pragma once
#include "Player.hpp"
#include "Connection.hpp"
#include "Field.hpp"
#include "Random.hpp"
#include "SerializedGame.hpp"

class Game {
	int turn{0};
	Phase phase;

	Player p1;
	Player p2;

	Random& random;

	Field field{};

public:
	Game(Connection p1con, Connection p2con, Random& random): phase(Phase::P1_SHUFFLE), p1(PlayerSide::P1, p1con, random), p2(PlayerSide::P2, p2con, random), random(random) {};

	void run();

	void notify(std::string event, json_t* extras);

	void destroyCard(Position cardPosition);

private:
	std::tuple<PlayerSide, Player&> getCurrentPlayer();
	bool checkRequirements(CardRequirements reqs);
	// returns true if the game is over
	bool onPhaseBegin();
	bool payCost(int n);
	SerializedGame serializeForPlayer(PlayerSide player, std::string method, json_t* extras);
	void playerWon(PlayerSide winner);
	void playerLost(PlayerSide winner);
};
