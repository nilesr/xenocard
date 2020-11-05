#include "Battle.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>


struct BattleEligibleCard {
	std::shared_ptr<BattleCard> card;
	int position;

	BattleEligibleCard(std::shared_ptr<BattleCard> card, int position): card(card), position(position) {};
};

bool cardPrioritizer(const BattleEligibleCard& a, const BattleEligibleCard& b) {
	static const std::unordered_map<AttackPattern, int> priorities {
		{AttackPattern::HAND, 0},
		{AttackPattern::BALLISTIC, 1},
		{AttackPattern::SPREAD, 2},
		{AttackPattern::HOMING, 3},
		{AttackPattern::PENETRATING, 1}
	};
	const auto pa = *priorities.find(a.card->getAttackPattern());
	const auto pb = *priorities.find(b.card->getAttackPattern());
	if (pa != pb) return pa < pb;
	return a.position < b.position;
}

std::shared_ptr<BattleCard> castCard(std::shared_ptr<Card> card) {
	return std::dynamic_pointer_cast<BattleCard>(card);
}

struct BattleTargets {
	std::vector<std::shared_ptr<BattleCard>> cards;
	bool hitsDeck;

	BattleTargets(std::shared_ptr<Card> card): cards(std::vector{castCard(card)}), hitsDeck(false) {};
	BattleTargets(std::vector<std::shared_ptr<BattleCard>> cards): cards(cards), hitsDeck(false) {};
	BattleTargets(bool hitsDeck): hitsDeck(hitsDeck) {};
	BattleTargets(): hitsDeck(false) {};
};

BattleTargets findOpponents(BattleEligibleCard card, PlayerField& enemyField) {
	const auto ap = card.card->getAttackPattern();
	if (ap == AttackPattern::HAND) {
		if (card.position == 0) {
			const auto opp = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 1);
			return opp.has_value() ? BattleTargets{*opp} : BattleTargets{};
		}
		if (card.position == 1) {
			const auto opp = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 0);
			return opp.has_value() ? BattleTargets{*opp} : BattleTargets{};
		}
		return BattleTargets{};
	}
	if (ap == AttackPattern::BALLISTIC || ap == AttackPattern::PENETRATING) {
		if (card.position == 0 || card.position == 3) {
			const auto opp1 = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 1);
			if (opp1.has_value()) return BattleTargets{*opp1};
			const auto opp2 = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 2);
			if (opp2.has_value()) return BattleTargets{*opp2};
		}
		if (card.position == 1 || card.position == 4) {
			const auto opp1 = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 0);
			if (opp1.has_value()) return BattleTargets{*opp1};
			const auto opp2 = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 3);
			if (opp2.has_value()) return BattleTargets{*opp2};
		}
		return BattleTargets{};
	}
	if (ap == AttackPattern::SPREAD) {
		std::vector<std::shared_ptr<BattleCard>> opps;
		for (int i = 0; i < 4; i++) {
			const auto opp = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, i);
			if (opp.has_value()) {
				opps.push_back(castCard(*opp));
			}
		}
		return BattleTargets{opps};
	}
	if (ap == AttackPattern::HOMING) {
		return BattleTargets{true};
	}
	throw std::logic_error{"unexpected attack pattern in findOpponents"};
}

void doBattle(Player& attacker, Player& enemy, PlayerField& attackerField, PlayerField& enemyField) {
	std::vector<BattleEligibleCard> cards{};
	for (auto i = 0; i < 4; i++) {
		const auto& card = attackerField.findCard(FieldSegmentEnum::BATTLEFIELD, i);
		if (card.has_value()) {
			cards.push_back(BattleEligibleCard{std::dynamic_pointer_cast<BattleCard>(*card), i});
		}
	}
	std::sort(cards.begin(), cards.end(), cardPrioritizer);
	// TODO add lvl 1 jr or lvl 10 jr if they exist in the enemy battlefield.
	for (const auto& card : cards) {
		// TODO proper implementation of penetrating damage
		auto opponents = findOpponents(card, enemyField);
		for (const auto opp_card : opponents.cards) {
			auto damage = card.card->getDamageFor(opp_card);
			std::cout << card.card->getName() << " dealing " << damage << " to card " << opp_card->getName() << std::endl;
			opp_card->takeDamage(damage);
		}
		if (opponents.hitsDeck) {
			auto damage = card.card->getDamageFor(std::nullopt);
			std::cout << "Would hit deck for: " << damage << std::endl;
			// TODO
			// enemy.takeDamage(damage);
		}
	}
}
