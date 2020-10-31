#pragma once
#include <map>
#include <string>
#include <memory>
#include <optional>
#include <jansson.h>
#include <vector>

enum class CardType {
	BATTLE,
	BATTLE_WEAPON,
	EVENT,
	SITUATION,
};

std::string cardTypeToString(CardType cardType);

enum class BattleCardType {
	HUMAN,
	REALIAN,
	MECH,
	GNOSIS,
};

std::string battleCardTypeToString(BattleCardType battleCardType);

enum class AttackPattern {
	HAND,
	BALLISTIC,
};


using CardRequirements = std::map<std::optional<BattleCardType>, int>;

class Card {
public:
	virtual std::string getName() = 0;
	virtual CardType getType() = 0;
	virtual CardRequirements getRequirements() = 0;
	virtual ~Card() {};
	json_t* serialize();
protected:
	virtual void serialize(json_t* obj);
};

class WeaponCard: public Card {
public:
	CardType getType() override {
		return CardType::BATTLE_WEAPON;
	};
	virtual AttackPattern getAttackPattern() = 0;
	int attack;
};

class BattleCard: public Card {
public:
	CardType getType() override {
		return CardType::BATTLE;
	}
	virtual BattleCardType getBattleType() = 0;
	// TODO: getAttack
	AttackPattern getAttackPattern();
	std::optional<std::shared_ptr<WeaponCard>> weapon;
	virtual void setE() { this->e = 1; };
	virtual void unsetE() { this->e = 0; };
	virtual bool getE() { return this->e; };
protected:
	int e = 0;
	virtual AttackPattern getDefaultAttackPattern() = 0;

	virtual void serialize(json_t* obj) override;

	int max_health;
	int health;
};


class EventCard: public Card {
	CardType getType() override {
		return CardType::EVENT;
	};
};

json_t* serializeCards(std::vector<std::shared_ptr<Card>> cards);
