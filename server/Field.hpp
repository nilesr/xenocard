#pragma once
#include "Card.hpp"
#include "Player.hpp"
#include <jansson.h>

enum class FieldSegment {
	BATTLEFIELD,
	STANDBY,
	SITUATION,
};

class Field {
private:
	std::optional<std::shared_ptr<Card>> P1_BATTLEFIELD_TOP_LEFT;
	std::optional<std::shared_ptr<Card>> P1_BATTLEFIELD_TOP_RIGHT;
	std::optional<std::shared_ptr<Card>> P1_BATTLEFIELD_BOTTOM_LEFT;
	std::optional<std::shared_ptr<Card>> P1_BATTLEFIELD_BOTTOM_RIGHT;
	std::optional<std::shared_ptr<Card>> P1_STANDBY_1;
	std::optional<std::shared_ptr<Card>> P1_STANDBY_2;
	std::optional<std::shared_ptr<Card>> P1_STANDBY_3;
	std::optional<std::shared_ptr<Card>> P1_STANDBY_4;
	std::optional<std::shared_ptr<Card>> P1_SITUATION_1;
	std::optional<std::shared_ptr<Card>> P1_SITUATION_2;
	std::optional<std::shared_ptr<Card>> P1_SITUATION_3;
	std::optional<std::shared_ptr<Card>> P1_SITUATION_4;

	std::optional<std::shared_ptr<Card>> P2_BATTLEFIELD_TOP_LEFT;
	std::optional<std::shared_ptr<Card>> P2_BATTLEFIELD_TOP_RIGHT;
	std::optional<std::shared_ptr<Card>> P2_BATTLEFIELD_BOTTOM_LEFT;
	std::optional<std::shared_ptr<Card>> P2_BATTLEFIELD_BOTTOM_RIGHT;
	std::optional<std::shared_ptr<Card>> P2_STANDBY_1;
	std::optional<std::shared_ptr<Card>> P2_STANDBY_2;
	std::optional<std::shared_ptr<Card>> P2_STANDBY_3;
	std::optional<std::shared_ptr<Card>> P2_STANDBY_4;
	std::optional<std::shared_ptr<Card>> P2_SITUATION_1;
	std::optional<std::shared_ptr<Card>> P2_SITUATION_2;
	std::optional<std::shared_ptr<Card>> P2_SITUATION_3;
	std::optional<std::shared_ptr<Card>> P2_SITUATION_4;

public:
	std::optional<std::shared_ptr<Card>>& findCard(PlayerSide side, FieldSegment segment, int index);

	json_t* serialize();

};
