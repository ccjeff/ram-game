#pragma once
#include "Items.hpp"

class SphereIntersection : public Items {
public:
  SphereIntersection(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs);
  
  void on_bullet_destroyed(Bullet* b) override;
};