#include "Multithreading.hpp"
#include "GameState.hpp"
#include <iostream>

using namespace std;

Multithreading::Multithreading(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs)
    : Items(player, pos, sprite, gs) {};

void Multithreading::on_shoot(Bullet *b) {
	const glm::vec2& vel = b->get_vel();
	cout << b->get_vel().x << " " << b->get_vel().y << endl;

	float vel1 = -0.1f, vel2 = -0.1f;
	if(vel.x < 0) {
		vel1 *= -1.0f;
	}
	if(vel.y < 0) {
		vel2 *= -1.0f;
	}

	gs->bullets.emplace_back(new Bullet(b->get_pos(), vel + glm::vec2(vel1, -vel2)));
	gs->bullets.emplace_back(new Bullet(b->get_pos(), vel + glm::vec2(-vel1, vel2)));

	cout << gs->bullets.back()->get_vel().x << " " << gs->bullets.back()->get_vel().y << endl;

}
