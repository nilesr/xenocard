#pragma once
#include "../Card.hpp"

class Realian: public BattleCard {
	std::string getName() override {
		return "Battle Realian";
	}
	BattleCardType getBattleType() override {
		return BattleCardType::REALIAN;
	}

	CardRequirements getRequirements() override {
		return {};
	}

	AttackPattern getDefaultAttackPattern() override {
		return AttackPattern::HAND;
	};

	std::array<int, 3> getImageLocation() override {
		return {1, 2, 7};
	}
};

