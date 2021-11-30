#pragma once
#include "Items.hpp"

class Debugger : public Items {
public:
  Debugger(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs);
  
  void on_shoot(Bullet* b) override;
};