#pragma once
#include "Items.hpp"
#include "Sprites.hpp"
#include <vector>

struct BulletExplosion {
  glm::vec2 location;
  float elapsed = 0.f;
  float radius = 1.f;
  void draw(glm::vec2 camera_center, std::vector<Vertex> &vertices);
};

class SphereIntersection : public Items {
public:
  SphereIntersection(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs);
  
  void on_bullet_destroyed(Bullet* b) override;
};