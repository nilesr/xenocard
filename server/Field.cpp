#include "Field.hpp"

json_t* Field::serialize() {
	auto obj = json_object();
	auto p1_battlefield = json_pack("[o?o?o?o?]",
			(this->P1_BATTLEFIELD_TOP_LEFT.has_value() ? (*this->P1_BATTLEFIELD_TOP_LEFT)->serialize() : nullptr),
			(this->P1_BATTLEFIELD_TOP_RIGHT.has_value() ? (*this->P1_BATTLEFIELD_TOP_RIGHT)->serialize() : nullptr),
			(this->P1_BATTLEFIELD_BOTTOM_LEFT.has_value() ? (*this->P1_BATTLEFIELD_BOTTOM_LEFT)->serialize() : nullptr),
			(this->P1_BATTLEFIELD_BOTTOM_RIGHT.has_value() ? (*this->P1_BATTLEFIELD_BOTTOM_RIGHT)->serialize() : nullptr)
		);
	auto p1_standby = json_pack("[o?o?o?o?]",
			(this->P1_STANDBY_1.has_value() ? (*this->P1_STANDBY_1)->serialize() : nullptr),
			(this->P1_STANDBY_2.has_value() ? (*this->P1_STANDBY_2)->serialize() : nullptr),
			(this->P1_STANDBY_3.has_value() ? (*this->P1_STANDBY_3)->serialize() : nullptr),
			(this->P1_STANDBY_4.has_value() ? (*this->P1_STANDBY_4)->serialize() : nullptr)
		);
	auto p1_situation = json_pack("[o?o?o?o?]",
			(this->P1_SITUATION_1.has_value() ? (*this->P1_SITUATION_1)->serialize() : nullptr),
			(this->P1_SITUATION_2.has_value() ? (*this->P1_SITUATION_2)->serialize() : nullptr),
			(this->P1_SITUATION_3.has_value() ? (*this->P1_SITUATION_3)->serialize() : nullptr),
			(this->P1_SITUATION_4.has_value() ? (*this->P1_SITUATION_4)->serialize() : nullptr)
		);
	auto p1 = json_pack("{s:o, s:o, s:o}",
			"BATTLEFIELD", p1_battlefield,
			"STANDBY", p1_standby,
			"SITUATION", p1_situation
		);

	auto p2_battlefield = json_pack("[o?o?o?o?]",
			(this->P2_BATTLEFIELD_TOP_LEFT.has_value() ? (*this->P2_BATTLEFIELD_TOP_LEFT)->serialize() : nullptr),
			(this->P2_BATTLEFIELD_TOP_RIGHT.has_value() ? (*this->P2_BATTLEFIELD_TOP_RIGHT)->serialize() : nullptr),
			(this->P2_BATTLEFIELD_BOTTOM_LEFT.has_value() ? (*this->P2_BATTLEFIELD_BOTTOM_LEFT)->serialize() : nullptr),
			(this->P2_BATTLEFIELD_BOTTOM_RIGHT.has_value() ? (*this->P2_BATTLEFIELD_BOTTOM_RIGHT)->serialize() : nullptr)
		);
	auto p2_standby = json_pack("[o?o?o?o?]",
			(this->P2_STANDBY_1.has_value() ? (*this->P2_STANDBY_1)->serialize() : nullptr),
			(this->P2_STANDBY_2.has_value() ? (*this->P2_STANDBY_2)->serialize() : nullptr),
			(this->P2_STANDBY_3.has_value() ? (*this->P2_STANDBY_3)->serialize() : nullptr),
			(this->P2_STANDBY_4.has_value() ? (*this->P2_STANDBY_4)->serialize() : nullptr)
		);
	auto p2_situation = json_pack("[o?o?o?o?]",
			(this->P2_SITUATION_1.has_value() ? (*this->P2_SITUATION_1)->serialize() : nullptr),
			(this->P2_SITUATION_2.has_value() ? (*this->P2_SITUATION_2)->serialize() : nullptr),
			(this->P2_SITUATION_3.has_value() ? (*this->P2_SITUATION_3)->serialize() : nullptr),
			(this->P2_SITUATION_4.has_value() ? (*this->P2_SITUATION_4)->serialize() : nullptr)
		);
	auto p2 = json_pack("{s:o, s:o, s:o}",
			"BATTLEFIELD", p2_battlefield,
			"STANDBY", p2_standby,
			"SITUATION", p2_situation
		);

	json_object_set(obj, "p1", p1);
	json_object_set(obj, "p2", p2);
	return obj;
}
