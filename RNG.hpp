#pragma once
#include "Items.hpp"

class RNG : public Items {
public:
  RNG(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs);
  
  void on_shoot(Bullet* b) override;
};