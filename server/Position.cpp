#include "Position.hpp"
#include "Json.hpp"
#include <stdexcept>

PlayerSide playerSideFromString(std::string in) {
	if (in == "P1") return PlayerSide::P1;
	if (in == "P2") return PlayerSide::P2;
	throw std::logic_error("Invalid player side");
}

FieldSegment fieldSegmentFromString(std::string in) {
	if (in == "BATTLEFIELD") return FieldSegment::BATTLEFIELD;
	if (in == "STANDBY") return FieldSegment::STANDBY;
	if (in == "SITUATION") return FieldSegment::SITUATION;
	throw std::logic_error("Invalid field segment");
}

Position positionFromJson(json_t* obj) {
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
