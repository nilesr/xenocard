#include "Battle.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>


struct BattleEligibleCard {
	std::shared_ptr<BattleCard> card;
	int position;

	BattleEligibleCard(std::shared_ptr<BattleCard> card, int position): card(card), position(position) {};
};

struct TargetedCard {
	std::shared_ptr<BattleCard> card;
	int position;

	TargetedCard(std::shared_ptr<BattleCard> card, int position): card(card), position(position) {};
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
	std::vector<TargetedCard> cards;
	bool hitsDeck;

	BattleTargets(std::shared_ptr<Card> card, int position): cards(std::vector{TargetedCard{castCard(card), position}}), hitsDeck(false) {};
	BattleTargets(std::vector<TargetedCard> cards): cards(cards), hitsDeck(false) {};
	BattleTargets(bool hitsDeck): hitsDeck(hitsDeck) {};
	BattleTargets(): hitsDeck(false) {};
};

BattleTargets findOpponents(BattleEligibleCard card, PlayerField& enemyField) {
	const auto ap = card.card->getAttackPattern();
	if (ap == AttackPattern::HAND) {
		if (card.position == 0) {
			const auto opp = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 1);
			return opp.has_value() ? BattleTargets{*opp, 1} : BattleTargets{};
		}
		if (card.position == 1) {
			const auto opp = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 0);
			return opp.has_value() ? BattleTargets{*opp, 0} : BattleTargets{};
		}
		return BattleTargets{};
	}
	if (ap == AttackPattern::BALLISTIC || ap == AttackPattern::PENETRATING) {
		if (card.position == 0 || card.position == 3) {
			const auto opp1 = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 1);
			if (opp1.has_value()) return BattleTargets{*opp1, 1};
			const auto opp2 = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 3);
			if (opp2.has_value()) return BattleTargets{*opp2, 3};
		}
		if (card.position == 1 || card.position == 2) {
			const auto opp1 = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 0);
			if (opp1.has_value()) return BattleTargets{*opp1, 0};
			const auto opp2 = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, 2);
			if (opp2.has_value()) return BattleTargets{*opp2, 2};
		}
		return BattleTargets{true};
	}
	if (ap == AttackPattern::SPREAD) {
		std::vector<TargetedCard> opps;
		for (int i = 0; i < 4; i++) {
			const auto opp = enemyField.findCard(FieldSegmentEnum::BATTLEFIELD, i);
			if (opp.has_value()) {
				opps.push_back(TargetedCard{castCard(*opp), i});
			}
		}
		return BattleTargets{opps};
	}
	if (ap == AttackPattern::HOMING) {
		return BattleTargets{true};
	}
	throw std::logic_error{"unexpected attack pattern in findOpponents"};
}

void doBattle(Game& game, Player& attacker, Player& enemy, PlayerField& attackerField, PlayerField& enemyField) {
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
		const auto source_position = Position{attacker.which, FieldSegmentEnum::BATTLEFIELD, card.position};
		game.notify("card_will_do_battle", json_pack("{s:o}",
			"battlefield_position", serializePosition(source_position)));

		// TODO proper implementation of penetrating damage
		auto opponents = findOpponents(card, enemyField);
		for (const auto opp_card : opponents.cards) {
			auto damage = card.card->getDamageFor(opp_card.card);
			std::cout << card.card->getName() << " dealing " << damage << " to card " << opp_card.card->getName() << std::endl;

			const auto target_position = Position{enemy.which, FieldSegmentEnum::BATTLEFIELD, opp_card.position};

			const auto remaining_health = opp_card.card->takeDamage(damage);
			if (remaining_health <= 0) {
				game.destroyCard(target_position);
			}

			game.notify("damage", json_pack("{s:o, s:o, s:i}",
				"source", serializePosition(source_position),
				"target", serializePosition(target_position),
				"damage", damage));
		}
		if (opponents.hitsDeck) {
			auto damage = card.card->getDamageFor(std::nullopt);
			std::cout << "Will hit deck for: " << damage << std::endl;

			enemy.payCost(damage);

			game.notify("damage_deck", json_pack("{s:o, s:s, s:i}",
				"source", serializePosition(source_position),
				"target", playerSideToString(enemy.which).c_str(),
				"damage", damage));
		}
	}
}
