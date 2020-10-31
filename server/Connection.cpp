#include "Connection.hpp"
#include <stdio.h>
#include <unistd.h>
#include <jansson.h>
#include <cstring>
#include <string>
#include <iostream>

std::optional<std::string> getString(json_t* json, const char* key) {
	json_t* obj = json_object_get(json, key);
	if (!obj) return std::nullopt;
	auto charp = json_string_value(obj);
	if (!charp) return std::nullopt;
	auto str = std::string{charp};
	json_decref(obj);
	return str;
}

class defer_decref {
	json_t* json;
public:
	defer_decref(json_t* json): json(json) {};
	~defer_decref() {
		json_decref(json);
	}
};

std::unique_ptr<Instruction> Connection::readInstruction() {
	unsigned long i = 0;
	char buf[4096];
	char c;
	while ((read(this->fd, &c, 1)) > 0 && c != '\n' && c != '\r' && i < sizeof(buf)) {
		std::cout << c;
		buf[i++] = c;
	}
	std::cout << std::endl;
	json_error_t error;
	json_t* json = json_loadb(buf, i, 0, &error);
	if (!json) throw std::logic_error(error.text);
	defer_decref g{json};
	auto meth = getString(json, "method");
	if (meth == "set_phase") {
		auto next_phase = getString(json, "phase");
		if (!next_phase.has_value()) {
			throw std::logic_error("Required key phase wasn't found");
		}
		auto phase = phaseFromString(*next_phase);
		return std::make_unique<SetPhaseInstruction>(
			phase
		);
	}
	if (meth == "shuffle") {
		return std::make_unique<ShuffleInstruction>();
	}

	throw std::logic_error("Unknown method");
}

void Connection::sendError(std::string what) {
	json_t* obj = json_object();
	json_object_set(obj, "type", json_string("error"));
	json_object_set(obj, "error", json_string(what.c_str()));
	char* stringified = json_dumps(obj, 0);
	int written = write(this->fd, stringified, strlen(stringified));
	write(this->fd, "\n", 1);
	std::cout << "Wrote " << written << " bytes back to the client" << std::endl;
	free(stringified);
	json_decref(obj);
}

void Connection::sendGame(SerializedGame game) {
	auto packed = json_pack("{s:s, s:i, s:s, s:o, s:o, s:i, s:i, s:i}",
			"player", game.player.c_str(),
			"turn", game.turn,
			"phase", game.phase.c_str(),
			"field", game.field,
			"hand", game.hand,
			"deckSize", game.deckSize,
			"enemyHandSize", game.enemyHandSize,
			"enemyDeckSize", game.enemyDeckSize
		);
	char* stringified = json_dumps(packed, 0);
	int written = write(this->fd, stringified, strlen(stringified));
	write(this->fd, "\n", 1);
	std::cout << "Wrote " << written << " bytes back to the client" << std::endl;
	free(stringified);
	json_decref(packed);
}
