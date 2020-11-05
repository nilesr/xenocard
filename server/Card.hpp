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
	BUNNY,
};

std::string battleCardTypeToString(BattleCardType battleCardType);

enum class AttackPattern {
	HAND,
	BALLISTIC,
	SPREAD,
	HOMING,
	PENETRATING,
};

std::string attackPatternToString(AttackPattern attackPattern);

using CardRequirements = std::map<std::optional<BattleCardType>, int>;

class Card {
public:
	virtual std::string getName() = 0;
	virtual CardType getType() = 0;
	virtual CardRequirements getRequirements() = 0;
	virtual int getCost() { return 0; };
	virtual ~Card() {};
	virtual std::array<int, 3> getImageLocation() = 0;
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
	AttackPattern getAttackPattern();
	std::optional<std::shared_ptr<WeaponCard>> weapon;
	virtual void setE() { this->e = true; };
	virtual void unsetE() { this->e = false; };
	virtual bool getE() { return this->e; };
	virtual bool countsTowardsLimit() { return this->getBattleType() != BattleCardType::REALIAN; };
	virtual int getMaxHealth() = 0;
	int getHealth() { return this->health; };
	// for std::nullopt, return the damage that should be dealt to the opponents deck
	virtual int getDamageFor(std::optional<std::shared_ptr<BattleCard>> other) = 0;
	// returns the amount of health remaining, or 0 if the card is now dead
	virtual int takeDamage(int damage) {
		this->health -= damage;
		return this->health < 0 ? 0 : this->health;
	};
protected:
	bool e = 0;
	int health;

	BattleCard(int max_health): health(max_health) {};
	virtual AttackPattern getDefaultAttackPattern() = 0;

	virtual void serialize(json_t* obj) override;
};


class EventCard: public Card {
	CardType getType() override {
		return CardType::EVENT;
	};
};

class SituationCard: public Card {
	CardType getType() override {
		return CardType::SITUATION;
	};
};

json_t* serializeCards(std::vector<std::shared_ptr<Card>> cards);
