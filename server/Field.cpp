#include "Field.hpp"
#include <stdexcept>

json_t* Battlefield::serialize() {
	return json_pack("[o?o?o?o?]",
			(this->TOP_LEFT.has_value() ? (*this->TOP_LEFT)->serialize() : nullptr),
			(this->TOP_RIGHT.has_value() ? (*this->TOP_RIGHT)->serialize() : nullptr),
			(this->BOTTOM_LEFT.has_value() ? (*this->BOTTOM_LEFT)->serialize() : nullptr),
			(this->BOTTOM_RIGHT.has_value() ? (*this->BOTTOM_RIGHT)->serialize() : nullptr)
		);
}

json_t* FieldSegment::serialize() {
	return json_pack("[o?o?o?o?]",
			(this->cards[0].has_value() ? (*this->cards[0])->serialize() : nullptr),
			(this->cards[1].has_value() ? (*this->cards[1])->serialize() : nullptr),
			(this->cards[2].has_value() ? (*this->cards[2])->serialize() : nullptr),
			(this->cards[3].has_value() ? (*this->cards[3])->serialize() : nullptr)
		);
}

json_t* PlayerField::serialize() {
	return json_pack("{s:o, s:o, s:o}",
			"BATTLEFIELD", this->battlefield.serialize(),
			"STANDBY", this->standby.serialize(),
			"SITUATION", this->situation.serialize()
		);
}

json_t* Field::serialize() {
	auto obj = json_object();
	auto p1 = this->p1.serialize();
	auto p2 = this->p2.serialize();

	json_object_set(obj, "p1", p1);
	json_object_set(obj, "p2", p2);
	return obj;
}

std::optional<std::shared_ptr<Card>>& Battlefield::findCard(int index) {
	switch (index) {
	case 0:
		return this->TOP_LEFT;
	case 1:
		return this->TOP_RIGHT;
	case 2:
		return this->BOTTOM_LEFT;
	case 3:
		return this->BOTTOM_RIGHT;
	}
	throw std::logic_error("Invalid index into the battlefield (must be 0, 1, 2 or 3 for top left, top right, buttom left or bottom right)");
}

std::optional<std::shared_ptr<Card>>& FieldSegment::findCard(int index) {
	return cards[index];
}

std::optional<std::shared_ptr<Card>>& PlayerField::findCard(FieldSegmentEnum segment, int index) {
	switch (segment) {
	case FieldSegmentEnum::STANDBY:
		return this->standby.findCard(index);
	case FieldSegmentEnum::BATTLEFIELD:
		return this->battlefield.findCard(index);
	case FieldSegmentEnum::SITUATION:
		return this->situation.findCard(index);
	}
	throw std::logic_error("Invalid field segment, must be STANDBY, BATTLEFIELD or SITUATION");
}

std::optional<std::shared_ptr<Card>>& Field::findCard(Position pos) {
	if (pos.playerSide == PlayerSide::P1) {
		return this->p1.findCard(pos.segment, pos.index);
	} else {
		return this->p2.findCard(pos.segment, pos.index);
	}
}

