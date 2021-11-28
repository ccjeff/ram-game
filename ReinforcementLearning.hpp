#pragma once
#include <glm/glm.hpp>
#include "Items.hpp"

class ReinforcementLearning : public Items {
public:
  ReinforcementLearning(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, std::shared_ptr<GameState> gs);
  
  void on_kill() override;
};
