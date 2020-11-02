#pragma once
#include "../Card.hpp"

class Curry: public EventCard {
	std::string getName() override {
		return "Curry";
	}

	CardRequirements getRequirements() override {
		return {}; // TODO
	}

	std::array<int, 3> getImageLocation() override {
		return {2, 1, 5};
	}
};
