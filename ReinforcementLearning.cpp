#include "ReinforcementLearning.hpp"

ReinforcementLearning::ReinforcementLearning(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, std::shared_ptr<GameState> gs) 
	: Items(player, pos, sprite, gs) {};

void ReinforcementLearning::on_kill() {
	if(player->get_hp() < 7.0f)
		player->add_hp(1.0f);
}
