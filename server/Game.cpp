#include "Game.hpp"
#include <iostream>

void Game::notify(std::string event, json_t* extras) {
	if (extras == nullptr) extras = json_object();
	json_object_set(extras, "event", json_string(event.c_str()));
	this->p1.notify(extras);
	this->p2.notify(extras);
	json_decref(extras);
}

void Game::onPhaseBegin() {
	auto [current_player, plr] = this->getCurrentPlayer();
	if (this->phase == Phase::P1_DRAW) {
		this->turn++;
	}
	if (this->phase == Phase::P1_DRAW || this->phase == Phase::P2_DRAW) {
		plr.drawCard();
		this->notify("draw_card", json_pack("{s:s}", "side", playerSideToString(current_player).c_str()));
	}
	if (this->phase == Phase::P1_ADJUST || this->phase == Phase::P2_ADJUST) {
		for (int i = 0; i < 4; i++) {
			auto& maybe_card = this->field.findCard(Position{current_player, FieldSegmentEnum::STANDBY, i});
			if (maybe_card.has_value()) {
				auto battleCardPtr = std::dynamic_pointer_cast<BattleCard>(*maybe_card);
				battleCardPtr->unsetE();
			}
		}
	}
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
	auto shuffles = 0;
	while (true) {
		this->sendState();
		auto [current_player, plr] = this->getCurrentPlayer();
		auto instruction = plr.recvInstruction();
		switch (instruction->getType()) {
		case InstructionType::SHUFFLE:
			{
				if ((this->phase == Phase::P1_SHUFFLE || this->phase == Phase::P2_SHUFFLE) && shuffles < 3) {
					plr.dealInitialHand();
					shuffles++;
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
				if (this->phase != Phase::P1_SET && this->phase != Phase::P2_SET) {
					plr.sendError("Can only play cards during the set phase");
					break;
				}
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
					// TODO CHECK REQUIREMENTS
					if (!target_slot.has_value() || (*target_slot)->getType() != CardType::BATTLE) {
						plr.sendError("Weapon cards must be played on top of a battle card");
						break;
					}
					// Good enough for me
					auto target_card = dynamic_cast<BattleCard*>(target_slot->get());
					// Discard the weapon already on the target card if they have one
					if (target_card->weapon.has_value()) {
						plr.junk.push_back(*target_card->weapon);
						target_card->weapon.reset();
					}
					plr.hand.erase(plr.hand.begin() + pi->handIndex);
					target_card->weapon = to_play_weapon;
					break;
				}
				if (to_play->getType() == CardType::BATTLE) {
					auto to_play_battle = std::dynamic_pointer_cast<BattleCard>(to_play);
					if (target_slot.has_value()) {
						plr.sendError("Can't place a battle card on top of another card. Move it out of the way first");
						break;
					}
					// TODO CHECK REQUIREMENTS
					plr.hand.erase(plr.hand.begin() + pi->handIndex);
					target_slot = to_play;
					to_play_battle->setE();
					break;
				}
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
				if (di->handIndex < 0 || di->handIndex >= 6) {
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

void Game::sendState() {
	auto p1game = this->serializeForPlayer(PlayerSide::P1);
	p1.sendState(p1game);
	auto p2game = this->serializeForPlayer(PlayerSide::P2);
	p2.sendState(p2game);
}

SerializedGame Game::serializeForPlayer(PlayerSide player) {
	auto& plr = player == PlayerSide::P1 ? this->p1 : this->p2;
	auto& enemy = player == PlayerSide::P1 ? this->p2 : this->p1;
	return SerializedGame{
		playerSideToString(player),
		this->turn,
		phaseToString(this->phase),
		this->field.serialize(),
		serializeCards(plr.hand),
		(int) plr.deck.size(),
		(int) enemy.hand.size(),
		(int) enemy.deck.size(),
	};
}

