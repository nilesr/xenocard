#pragma once
#include "../Card.hpp"

class Gun: public WeaponCard {
	std::string getName() override {
		return "Gun";
	}

	CardRequirements getRequirements() override {
		return {};
	}
	AttackPattern getAttackPattern() override {
		return AttackPattern::BALLISTIC;
	};
	std::array<int, 3> getImageLocation() override {
		return {1, 4, 9};
	}

};
