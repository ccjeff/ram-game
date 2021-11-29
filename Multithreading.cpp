#include "Multithreading.hpp"
#include <iostream>

Multithreading::Multithreading(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs)
    : Items(player, pos, sprite, gs) {};

void Multithreading::on_shoot(Bullet *b) {
	using namespace std;
	cout << b->get_vel().x << " " << b->get_vel().y << endl;
}
