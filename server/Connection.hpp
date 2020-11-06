#pragma once
#include "Instruction.hpp"
#include "Phase.hpp"
#include "SerializedGame.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <optional>
#include <memory>

class Connection {
	int fd;

public:
	Connection(int fd): fd(fd) {};

	std::unique_ptr<Instruction> readInstruction();
	void sendError(std::string what);
	void sendGame(SerializedGame game);
};
