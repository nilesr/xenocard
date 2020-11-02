#include "Connection.hpp"
#include <stdio.h>
#include <unistd.h>
#include <jansson.h>
#include <cstring>
#include <string>
#include <iostream>
#include "Json.hpp"

std::unique_ptr<Instruction> Connection::readInstruction() {
	unsigned long i = 0;
	char buf[4096];
	char c;
	while ((read(this->fd, &c, 1)) > 0 && c != '\n' && c != '\r' && i < sizeof(buf)) {
		std::cout << c;
		buf[i++] = c;
	}
	if (i == sizeof(buf)) {
		std::cout << "Probably hit the max buffer size reading in a message" << std::endl;
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
			throw std::logic_error("Required key phase wasn't found (or wasn't a string)");
		}
		auto phase = phaseFromString(*next_phase);
		return std::make_unique<SetPhaseInstruction>(
			phase
		);
	}
	if (meth == "shuffle") {
		return std::make_unique<ShuffleInstruction>();
	}
	if (meth == "play") {
		auto hand_index = getInt(json, "hand_index");
		if (!hand_index.has_value()) throw std::logic_error("Required key hand_index wasn't found (or wasn't an integer)");
		auto position = json_object_get(json, "position");
		if (position == nullptr || !json_is_object(position)) throw std::logic_error("Required key position wasn't found or wasn't an object");
		return std::make_unique<PlayInstruction>(*hand_index, positionFromJson(position));
	}
	if (meth == "discard") {
		auto hand_index = getInt(json, "hand_index");
		if (!hand_index.has_value()) throw std::logic_error("Required key hand_index wasn't found (or wasn't an integer)");
		return std::make_unique<DiscardInstruction>(*hand_index);
	}
	if (meth == "move") {
		auto start = json_object_get(json, "start");
		if (start == nullptr || !json_is_object(start)) throw std::logic_error("Required key 'start' wasn't found or wasn't an object");
		auto end = json_object_get(json, "end");
		if (end == nullptr || !json_is_object(end)) throw std::logic_error("Required key 'end' wasn't found or wasn't an object");
		return std::make_unique<MoveInstruction>(positionFromJson(start), positionFromJson(end));
	}

	throw std::logic_error("Unknown method");
}

void Connection::sendError(std::string what) {
	json_t* obj = json_object();
	json_object_set(obj, "event", json_string("error"));
	json_object_set(obj, "error", json_string(what.c_str()));
	char* stringified = json_dumps(obj, 0);
	int written = write(this->fd, stringified, strlen(stringified));
	write(this->fd, "\n", 1);
	std::cout << "Wrote " << written << " bytes back to the client" << std::endl;
	free(stringified);
	json_decref(obj);
}

void Connection::notify(json_t* what) {
	char* stringified = json_dumps(what, 0);
	int written = write(this->fd, stringified, strlen(stringified));
	write(this->fd, "\n", 1);
	std::cout << "Wrote " << written << " bytes back to the client" << std::endl;
	free(stringified);
}

void Connection::sendGame(SerializedGame game) {
	auto packed = json_pack("{s:s, s:s, s:i, s:s, s:o, s:o, s:o, s:i, s:i, s:i, s:i, s:i, s:i}",
			"event", "state",
			"player", game.player.c_str(),
			"turn", game.turn,
			"phase", game.phase.c_str(),
			"field", game.field,
			"hand", game.hand,
			"junk", game.junk,
			"deck_size", game.deckSize,
			"lost_size", game.lostSize,
			"enemy_hand_size", game.enemyHandSize,
			"enemy_deck_size", game.enemyDeckSize,
			"enemy_junk_size", game.enemyJunkSize,
			"enemy_lost_size", game.enemyLostSize
		);
	char* stringified = json_dumps(packed, 0);
	int written = write(this->fd, stringified, strlen(stringified));
	write(this->fd, "\n", 1);
	std::cout << "Wrote " << written << " bytes back to the client" << std::endl;
	free(stringified);
	json_decref(packed);
}
