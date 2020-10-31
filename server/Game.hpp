#pragma once
#include "Player.hpp"
#include "Connection.hpp"

class Game {
	Player p1;
	Player p2;

	Phase phase;

public:
	Game(Connection p1con, Connection p2con): p1(p1con), p2(p2con), phase(Phase::P1_DRAW) {};

	void run();
};
