#include "P_NP.hpp"

P_NP::P_NP(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, std::shared_ptr<GameState> gs)
    : Items(player, pos, sprite, gs) {};

void P_NP::on_shoot(Bullet *b) {
    b->set_damage(999.0f);
}
