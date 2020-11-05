#pragma once
#include "../Card.hpp"

class Mech: public BattleCard {
	std::string getName() override {
		return "AWGS";
	}

	BattleCardType getBattleType() override {
		return BattleCardType::MECH;
	}

	int getCost() override {
		return 2;
	}

	int getMaxHealth() override {
		return 4;
	}

	int getDamageFor(std::optional<std::shared_ptr<BattleCard>> other) override {
		return 2;
	}


	CardRequirements getRequirements() override {
		return {{BattleCardType::HUMAN, 1}};
	}

	AttackPattern getDefaultAttackPattern() override {
		return AttackPattern::BALLISTIC;
	};

	std::array<int, 3> getImageLocation() override {
		return {1, 3, 6};
	}

public:
	Mech(): BattleCard(this->getMaxHealth()) {};

};

