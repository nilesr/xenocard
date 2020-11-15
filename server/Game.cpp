#include "Game.hpp"
#include "Battle.hpp"
#include <iostream>

void Game::notify(std::string event, json_t* extras) {
	if (extras != nullptr) {
		json_incref(extras);
	}
	auto p1game = this->serializeForPlayer(PlayerSide::P1, event, extras);
	this->p1.sendState(p1game);
	auto p2game = this->serializeForPlayer(PlayerSide::P2, event, extras);
	this->p2.sendState(p2game);
}

bool Game::checkRequirements(CardRequirements reqs) {
	auto [current_player, plr] = this->getCurrentPlayer();
	for (const auto card : this->field.iterateForPlayer(current_player)) {
		if (card->getType() != CardType::BATTLE) continue;
		auto battle_card = std::dynamic_pointer_cast<BattleCard>(card);
		if (battle_card->getE()) continue;
		const auto type = battle_card->getBattleType();
		// reqs[type] will insert the value as 0 if it doesn't exist
		if (reqs[type] > 0) {
			--reqs[type];
		} else {
			--reqs[std::nullopt];
		}
	}
	for (const auto& [k, v] : reqs) {
		if (v > 0) return false;
	}
	return true;
}


void Game::onPhaseBegin() {
	auto [current_player, plr] = this->getCurrentPlayer();
	if (this->phase == Phase::P1_DRAW) {
		this->turn++;
	}
	this->notify("phase", nullptr);
	if (this->phase == Phase::P1_DRAW || this->phase == Phase::P2_DRAW) {
		plr.drawCard();
		this->notify("draw_card", json_pack("{s:s}", "side", playerSideToString(current_player).c_str()));
	}
	if (this->phase == Phase::P1_BATTLE || this->phase == Phase::P2_BATTLE) {
		for (int i = 0; i < 4; i++) {
			auto& maybe_card = this->field.findCard(Position{current_player, FieldSegmentEnum::STANDBY, i});
			if (maybe_card.has_value()) {
				auto battleCardPtr = std::dynamic_pointer_cast<BattleCard>(*maybe_card);
				battleCardPtr->unsetE();
			}
		}
		auto& other_player = current_player == PlayerSide::P1 ? this->p2 : this->p1;
		auto& field = current_player == PlayerSide::P1 ? this->field.p1 : this->field.p2;
		auto& enemy_field = current_player == PlayerSide::P1 ? this->field.p2 : this->field.p1;
		doBattle(*this, plr, other_player, field, enemy_field);
	}
}

void Game::payCost(int n) {
	auto [current_player, plr] = this->getCurrentPlayer();
	plr.payCost(n);
	this->notify("pay_cost", json_pack("{s:s, s:i}", "side", playerSideToString(current_player).c_str(), "n", n));
}

std::tuple<PlayerSide, Player&> Game::getCurrentPlayer() {
	auto current_player = (
		this->phase == Phase::P1_SHUFFLE ||
		this->phase == Phase::P1_DRAW ||
		this->phase == Phase::P1_MOVE ||
		this->phase == Phase::P1_EVENT ||
		this->phase == Phase::P1_SET ||
		this->phase == Phase::P2_BLOCK ||
		this->phase == Phase::P1_BATTLE ||
		this->phase == Phase::P1_ADJUST
	) ? PlayerSide::P1 : PlayerSide::P2;
	auto& plr = current_player == PlayerSide::P1 ? this->p1 : this->p2;
	return {current_player, plr};
}

void Game::run() {
	this->p1.dealInitialHand();
	this->p2.dealInitialHand();
	this->notify("game_start", nullptr);
	auto shuffles = 0;
	bool has_played_battle_card = false;
	bool has_played_situation_card = false;
	while (true) {
		auto [current_player, plr] = this->getCurrentPlayer();
		auto instruction = plr.recvInstruction();
		switch (instruction->getType()) {
		case InstructionType::SHUFFLE:
			{
				if ((this->phase == Phase::P1_SHUFFLE || this->phase == Phase::P2_SHUFFLE) && shuffles < 3) {
					plr.dealInitialHand();
					shuffles++;
					this->notify("shuffle", json_pack("{s:s}", "side", playerSideToString(current_player).c_str()));
				} else {
					plr.sendError("You may not shuffle at this time, either it is not that phase, or you have already shuffled 3 times");
				}
			}
			break;
		case InstructionType::SET_PHASE:
			{
				if ((this->phase == Phase::P1_ADJUST || this->phase == Phase::P2_ADJUST) && plr.hand.size() > 6) {
					plr.sendError("You must discard cards until your hand is of size 6 before you can end the current phase");
					break;
				}
				auto spi = dynamic_cast<SetPhaseInstruction*>(instruction.get());
				std::cout << "Would-be new phase: " << (int)spi->newPhase << std::endl;
				if (spi->newPhase == nextPhase(this->phase)) {
					this->phase = spi->newPhase;
					std::cout << "New phase: " << (int)this->phase << std::endl;
					if (this->phase == Phase::P2_SHUFFLE) {
						shuffles = 0;
					}
					has_played_battle_card = false;
					has_played_situation_card = false;
					this->onPhaseBegin();
					break;
				} else {
					plr.sendError("You cannot set that phase right now");
					break;
				}
			}
			break;
		case InstructionType::PLAY:
			{
				auto pi = dynamic_cast<PlayInstruction*>(instruction.get());
				if (pi->handIndex < 0 || pi->handIndex >= plr.hand.size()) {
					plr.sendError("Card at that index couldn't be found in your hand");
					break;
				}
				if (current_player != pi->position.playerSide) {
					plr.sendError("Cards must be placed on your side of the board");
					break;
				}
				auto to_play = plr.hand.at(pi->handIndex);
				if (to_play->getType() == CardType::EVENT) {
					plr.sendError("You can't play event cards onto the field, call use_card_effect instead");
					break;
				}
				if (to_play->getType() == CardType::SITUATION && pi->position.segment != FieldSegmentEnum::SITUATION) {
					plr.sendError("Situation cards must be played into the situation field segment");
					break;
				}
				if (to_play->getType() == CardType::BATTLE && pi->position.segment != FieldSegmentEnum::STANDBY) {
					plr.sendError("Battle cards must be played into the standby field segment");
					break;
				}
				if (to_play->getType() == CardType::BATTLE_WEAPON && pi->position.segment == FieldSegmentEnum::SITUATION) {
					plr.sendError("Weapon cards must be played into the standby or battlefield field segments");
					break;
				}
				auto& target_slot = this->field.findCard(pi->position);
				if (to_play->getType() == CardType::BATTLE_WEAPON) {
					auto to_play_weapon = std::dynamic_pointer_cast<WeaponCard>(to_play);
					if (this->phase != Phase::P1_SET && this->phase != Phase::P2_SET && this->phase != Phase::P1_MOVE && this->phase != Phase::P2_MOVE) {
						plr.sendError("Can only play weapon cards during the set or move phases");
						break;
					}
					if (!this->checkRequirements(to_play_weapon->getRequirements())) {
						plr.sendError("You don't meet the requirements to play this card now.");
						break;
					}
					if (!target_slot.has_value() || (*target_slot)->getType() != CardType::BATTLE) {
						plr.sendError("Weapon cards must be played on top of a battle card");
						break;
					}
					// TODO check that the target battle card can equip the weapon, (lvl 1 kos-mos, lvl 10 kos-mos, durandal, etc..)
					// Good enough for me
					auto target_card = dynamic_cast<BattleCard*>(target_slot->get());
					// Discard the weapon already on the target card if they have one
					if (target_card->weapon.has_value()) {
						plr.junk.push_back(*target_card->weapon);
						target_card->weapon.reset();
					}
					plr.hand.erase(plr.hand.begin() + pi->handIndex);
					target_card->weapon = to_play_weapon;
					this->payCost(target_card->getCost());
					break;
				}
				if (to_play->getType() == CardType::BATTLE) {
					if (this->phase != Phase::P1_SET && this->phase != Phase::P2_SET) {
						plr.sendError("Can only play battle cards during the set phase");
						break;
					}
					auto to_play_battle = std::dynamic_pointer_cast<BattleCard>(to_play);
					if (to_play_battle->countsTowardsLimit() && has_played_battle_card) {
						plr.sendError("You already played a battle card this turn.");
						break;
					}
					// TODO handle gnosis
					if (target_slot.has_value()) {
						plr.sendError("Can't place a battle card on top of another card. Move it out of the way first");
						break;
					}
					if (!this->checkRequirements(to_play_battle->getRequirements())) {
						plr.sendError("You don't meet the requirements to play this card now.");
						break;
					}
					plr.hand.erase(plr.hand.begin() + pi->handIndex);
					target_slot = to_play;
					to_play_battle->setE();
					if (to_play_battle->countsTowardsLimit()) {
						has_played_battle_card = true;
					}
					this->payCost(to_play->getCost());
					break;
				}
				if (to_play->getType() == CardType::SITUATION) {
					if (this->phase != Phase::P1_SET && this->phase != Phase::P2_SET) {
						plr.sendError("Can only play situation cards during the set phase");
						break;
					}
					auto to_play_situation = std::dynamic_pointer_cast<SituationCard>(to_play);
					if (has_played_situation_card) {
						plr.sendError("You already played a situation card this turn.");
						break;
					}
					if (target_slot.has_value()) {
						plr.sendError("Can't place a situation card on top of another card. Move it out of the way first");
						break;
					}
					if (!this->checkRequirements(to_play_situation->getRequirements())) {
						plr.sendError("You don't meet the requirements to play this card now.");
						break;
					}
					plr.hand.erase(plr.hand.begin() + pi->handIndex);
					target_slot = to_play;
					has_played_situation_card = true;
					this->payCost(to_play->getCost());
					break;
				}
			}
			break;
		case InstructionType::MOVE:
			{
				if (this->phase != Phase::P1_MOVE && this->phase != Phase::P2_MOVE) {
					plr.sendError("You must be in the move phase to move a card");
					break;
				}
				auto mi = dynamic_cast<MoveInstruction*>(instruction.get());
				if (mi->start.playerSide != current_player || mi->end.playerSide != current_player) {
					plr.sendError("You can only move around cards on your side of the field");
					break;
				}
				const auto startSituation = mi->start.segment == FieldSegmentEnum::SITUATION;
				const auto endSituation = mi->end.segment == FieldSegmentEnum::SITUATION;
				if ((startSituation && !endSituation) || (endSituation && !startSituation)) {
					plr.sendError("You are trying to move a card between the situation segment and the battlefield or standby segment, or vice versa");
					break;
				}
				auto& start_card = this->field.findCard(mi->start);
				if (!start_card.has_value()) {
					plr.sendError("You are trying to move from an empty spot, did you mean to swap start and end?");
					break;
				}
				auto& end_card = this->field.findCard(mi->end);
				std::swap(start_card, end_card);
				this->notify("move", json_pack("{s:o, s:o}", "start", serializePosition(mi->start), "end", serializePosition(mi->end)));
			}
			break;
		case InstructionType::DISCARD:
			{
				if (this->phase != Phase::P1_ADJUST && this->phase != Phase::P2_ADJUST) {
					plr.sendError("You must be in the adjust phase to discard a card");
					break;
				}
				if (plr.hand.size() <= 6) {
					plr.sendError("You may only discard a card if you have >6 cards in your hand");
					break;
				}
				auto di = dynamic_cast<DiscardInstruction*>(instruction.get());
				if (di->handIndex < 0 || di->handIndex >= plr.hand.size()) {
					plr.sendError("I couldn't find a card at that index in your hand");
					break;
				}
				plr.junk.push_back(plr.hand.at(di->handIndex));
				plr.hand.erase(plr.hand.begin() + di->handIndex);
				this->notify("discard_card", json_pack("{s:s}", "side", playerSideToString(current_player).c_str()));
			}
			break;
		default:
			plr.sendError("Unrecognized instruction type in run_game");
			break;
		}
		// Shouldn't put any code here because if the given instruction was set_phase, and it switched to another current_player,
		// the variables plr and current_player haven't been updated yet
	}
}

void Game::destroyCard(const Position cardPosition) {
	auto& card = this->field.findCard(cardPosition);
	if (card == std::nullopt) {
		throw std::logic_error{"Attempted to destroy a card that doesn't exist"};
	}
	if ((*card)->getType() != CardType::BATTLE) {
		throw std::logic_error{"Attempted to destroy a card that isn't a battle card"};
	}
	const std::shared_ptr<BattleCard> as_battle_card = std::dynamic_pointer_cast<BattleCard>(*card);
	auto& unlucky_player = cardPosition.playerSide == PlayerSide::P1 ? this->p1 : this->p2;
	if (as_battle_card->weapon != std::nullopt) {
		unlucky_player.junk.push_back(*as_battle_card->weapon);
		as_battle_card->weapon = std::nullopt;
	}
	unlucky_player.junk.push_back(*card);
	card = std::nullopt;
	this->notify("card_destroyed", json_pack("{s:o}", "position", serializePosition(cardPosition)));
}


SerializedGame Game::serializeForPlayer(PlayerSide player, std::string method, json_t* extras) {
	auto& plr = player == PlayerSide::P1 ? this->p1 : this->p2;
	auto& enemy = player == PlayerSide::P1 ? this->p2 : this->p1;
	return SerializedGame{
		method,
		extras,
		playerSideToString(player),
		this->turn,
		phaseToString(this->phase),
		this->field.serialize(),
		serializeCards(plr.hand),
		serializeCards(plr.junk),
		(int) plr.deck.size(),
		(int) plr.lost.size(),
		(int) enemy.hand.size(),
		(int) enemy.deck.size(),
		serializeCards(enemy.junk),
		(int) enemy.lost.size(),
	};
}

