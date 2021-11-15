#include "ReinforcementLearning.hpp"

ReinforcementLearning::ReinforcementLearning(std::shared_ptr<Player> player) : Items(player) {};

void ReinforcementLearning::on_kill() {
   player->add_hp(1.0f);
}
