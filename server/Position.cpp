#include "Position.hpp"
#include "Json.hpp"
#include <stdexcept>

PlayerSide playerSideFromString(std::string in) {
	if (in == "P1") return PlayerSide::P1;
	if (in == "P2") return PlayerSide::P2;
	throw std::logic_error("Invalid player side");
}

FieldSegmentEnum fieldSegmentFromString(std::string in) {
	if (in == "BATTLEFIELD") return FieldSegmentEnum::BATTLEFIELD;
	if (in == "STANDBY") return FieldSegmentEnum::STANDBY;
	if (in == "SITUATION") return FieldSegmentEnum::SITUATION;
	throw std::logic_error("Invalid field segment");
}

std::string fieldSegmentToString(FieldSegmentEnum in) {
	switch (in) {
		case FieldSegmentEnum::BATTLEFIELD: return "BATTLEFIELD";
		case FieldSegmentEnum::STANDBY: return "STANDBY";
		case FieldSegmentEnum::SITUATION: return "SITUATION";
	}
	throw std::logic_error("Invalid field segment");
}

Position positionFromJson(const json_t* const obj) {
	auto ps_str = getString(obj, "player_side");
	if (!ps_str.has_value()) throw std::logic_error("Missing player_side from position json (or not a string)");
	auto fs_str = getString(obj, "field_segment");
	if (!fs_str.has_value()) throw std::logic_error("Missing field_segment from position json (or not a string)");
	auto index = getInt(obj, "index");
	if (!index.has_value()) throw std::logic_error("Missing index from position json (or not an integer)");
	if (*index < 0 || *index > 3) throw std::logic_error("Index must be 0, 1, 2 or 3 (for the battlefield, this is top left, top right, bottom left, bottom right)");

	return Position{
		playerSideFromString(*ps_str),
		fieldSegmentFromString(*fs_str),
		*index
	};
}

json_t* serializePosition(Position p) {
	json_t* r = json_object();
	json_object_set(r, "player_side", json_string(playerSideToString(p.playerSide).c_str()));
	json_object_set(r, "field_segment", json_string(fieldSegmentToString(p.segment).c_str()));
	json_object_set(r, "index", json_integer(p.index));
	return r;
}
