#include "RubberDucky.hpp"
#include "GameState.hpp"

using namespace std;

RubberDucky::RubberDucky(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs)
    : Items(player, pos, sprite, gs) {};

void RubberDucky::on_shoot(Bullet* b) {
    b->get_rgb().emplace(RGB::Blue);
	b->set_damage(b->get_damage() + 0.25f);
}