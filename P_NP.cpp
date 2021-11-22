#include "P_NP.hpp"

P_NP::P_NP(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite) : Items(player, pos, sprite) {};

void P_NP::on_shoot(Bullet *b, std::vector<Enemy*>& enemies) {
    b->set_damage(999.0f);
}
