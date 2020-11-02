#pragma once
#include "../Card.hpp"

class Human: public BattleCard {
	std::string getName() override {
		return "Human";
	}

	BattleCardType getBattleType() override {
		return BattleCardType::HUMAN;
	}

	CardRequirements getRequirements() override {
		return {};
	}

	AttackPattern getDefaultAttackPattern() override {
		return AttackPattern::HAND;
	};

	std::array<int, 3> getImageLocation() override {
		return {1, 1, 8};
	}

};

