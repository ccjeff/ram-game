#include "Debugger.hpp"
#include "GameState.hpp"

using namespace std;

ThermalPaste::ThermalPaste(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs)
    : Items(player, pos, sprite, gs) {};

void ThermalPaste::on_shoot(Bullet* b) {
    b->get_rgb().emplace(RGB::Red);
	b->set_damage(b->get_damage() + 0.25f);
}