#include "P_NP.hpp"

P_NP::P_NP(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs)
    : Items(player, pos, sprite, gs) {};

void P_NP::on_shoot(Bullet *b) {
	b->get_rgb().emplace(RGB::Red);
	b->get_rgb().emplace(RGB::Green);
	b->get_rgb().emplace(RGB::Blue);
}
