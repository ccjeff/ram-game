#pragma once
#include <glm/glm.hpp>
#include "Items.hpp"

class RayTracing : public Items {
public:
  RayTracing(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite);
  
  void on_shoot(Bullet *b) override;
};
