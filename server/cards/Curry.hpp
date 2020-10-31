#pragma once
#include "../Card.hpp"

class Curry: public EventCard {
	std::string getName() override {
		return "Curry";
	}

	CardRequirements getRequirements() override {
		return {}; // TODO
	}
};
