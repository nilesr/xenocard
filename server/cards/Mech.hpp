#pragma once
#include "../Card.hpp"

class Mech: public BattleCard {
	std::string getName() override {
		return "AWGS";
	}

	BattleCardType getBattleType() override {
		return BattleCardType::MECH;
	}

	CardRequirements getRequirements() override {
		return {{BattleCardType::HUMAN, 1}};
	}

	AttackPattern getDefaultAttackPattern() override {
		return AttackPattern::BALLISTIC;
	};

};

