#include "ReinforcementLearning.hpp"

ReinforcementLearning::ReinforcementLearning(std::shared_ptr<Player> player, glm::vec2 pos) : Items(player, pos) {};

void ReinforcementLearning::on_kill() {
   player->add_hp(1.0f);
}
