#include "Multithreading.hpp"


Multithreading::Multithreading(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, std::shared_ptr<GameState> gs)
    : Items(player, pos, sprite, gs) {};

void Multithreading::on_shoot(Bullet *b, GameState* gs) {
    float smallest_distance = 99999.0f;

    b->set_auto_aim(true);
    if (b->get_auto_aim()== true) {
        for (auto e : enemies) {
            float distance = glm::distance(e->get_pos(), b->get_pos());
            if (distance < smallest_distance) {
                smallest_distance = distance;
                b->set_autoaim_target(e);
            }
        }
    }

}
