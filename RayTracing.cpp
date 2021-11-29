#include "RayTracing.hpp"

RayTracing::RayTracing(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs) 
	: Items(player, pos, sprite, gs) {};

void RayTracing::on_shoot(Bullet *b) {
   b->set_bounces(3);
}
