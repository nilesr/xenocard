#pragma once
#include "Connection.hpp"
#include "Instruction.hpp"
#include <memory>

class Player {
	Connection conn;

public:
	Player(Connection conn): conn(conn) {};
	std::unique_ptr<Instruction> recvInstruction();
	void sendError(std::string what);
};
