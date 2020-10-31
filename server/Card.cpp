#include "Card.hpp"
#include "Json.hpp"
#include <stdexcept>

AttackPattern BattleCard::getAttackPattern() {
	if (this->weapon.has_value()) {
		return (*this->weapon)->getAttackPattern();
	}
	return this->getDefaultAttackPattern();
}

std::string cardTypeToString(CardType cardType) {
	switch (cardType) {
		case CardType::BATTLE: return "BATTLE";
		case CardType::BATTLE_WEAPON: return "BATTLE_WEAPON";
		case CardType::EVENT: return "EVENT";
		case CardType::SITUATION: return "SITUATION";
	}
	throw std::logic_error{"should never get here"};
}

std::string battleCardTypeToString(BattleCardType battleCardType) {
	switch (battleCardType) {
		case BattleCardType::HUMAN: return "HUMAN";
		case BattleCardType::MECH: return "MECH";
		case BattleCardType::REALIAN: return "REALIAN";
		case BattleCardType::GNOSIS: return "GNOSIS";
	}
	throw std::logic_error{"should never get here"};
}


json_t* Card::serialize() {
	json_t* r = json_object();
	this->serialize(r);
	return r;
}

void Card::serialize(json_t* obj) {
	json_object_set(obj, "name", json_string(this->getName().c_str()));
	json_object_set(obj, "type", json_string(cardTypeToString(this->getType()).c_str()));
	json_t* (*int_to_json_int)(int) = [](int a){return json_integer((int) a);};
	json_object_set(obj, "requirements", map_to_obj(this->getRequirements(), battleCardTypeToString, int_to_json_int));
}

void BattleCard::serialize(json_t* obj) {
	this->Card::serialize(obj);
	json_object_set(obj, "battle_type", json_string(battleCardTypeToString(this->getBattleType()).c_str()));
	json_object_set(obj, "attack_pattern", json_string(this->getAttackPattern() == AttackPattern::HAND ? "HAND" : "BALLISTIC"));
	json_object_set(obj, "max_health", json_integer(this->max_health));
	json_object_set(obj, "health", json_integer(this->health));
	if (this->weapon.has_value()) {
		json_object_set(obj, "weapon", (*this->weapon)->serialize());
	}
}

json_t* serializeCards(std::vector<std::shared_ptr<Card>> cards) {
	json_t* arr = json_array();
	for (const auto& card : cards) {
		json_array_append_new(arr, card->serialize());
	}
	return arr;
}


