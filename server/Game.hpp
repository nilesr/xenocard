#pragma once
#include "Player.hpp"
#include "Connection.hpp"
#include "Field.hpp"
#include "Random.hpp"
#include "SerializedGame.hpp"

class Game {
	int turn{-1};
	Phase phase;

	Player p1;
	Player p2;

	Random& random;

	Field field{};

public:
	Game(Connection p1con, Connection p2con, Random& random): phase(Phase::P1_SHUFFLE), p1(PlayerSide::P1, p1con, random), p2(PlayerSide::P2, p2con, random), random(random) {};

	void run();

private:
	std::tuple<PlayerSide, Player&> getCurrentPlayer();
	void onPhaseBegin();
	void notify(std::string event, json_t* extras);
	void sendState();
	SerializedGame serializeForPlayer(PlayerSide player);

};
