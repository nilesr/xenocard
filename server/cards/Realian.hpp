#pragma once
#include "../Card.hpp"

class Realian: public BattleCard {
	std::string getName() override {
		return "Battle Realian";
	}

	BattleCardType getBattleType() override {
		return BattleCardType::REALIAN;
	}
	int getMaxHealth() override {
		return 2;
	}
	int getDamageFor(std::optional<std::shared_ptr<BattleCard>> other) override {
		return 1;
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

public:
	Realian(): BattleCard(this->getMaxHealth()) {};

};

