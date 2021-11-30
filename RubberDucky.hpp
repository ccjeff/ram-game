#pragma once
#include "Items.hpp"

class RubberDucky : public Items {
public:
  RubberDucky(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs);
  
  void on_shoot(Bullet* b) override;
};