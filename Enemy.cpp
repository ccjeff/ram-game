#include "Enemy.hpp"
#include "Dijkstra.hpp"
#include <iostream>

const glm::vec2& Enemy::get_pos() {
	return this->position;
}

const glm::vec2& Enemy::get_vel() {
	return this->velocity;
}

void Enemy::set_vel(glm::vec2 &vel) {
	this->velocity = vel;
}

float Enemy::get_hp() {
	return this->hp;
}

float Enemy::distance(const glm::vec2 &player_pos) {
	return glm::distance(this->position, player_pos);
}

float Enemy::get_width() {
	return this->width;
}

Sprite* Enemy::get_sprite() {
	return this->s;
}

void Dijkstra::on_shoot(Bullet *b, std::vector<Enemy*>& enemies) {
    float smallest_distance = 99999.0f;
	std::cout << "dijkstra item shoots init" << std::endl;
    b->set_auto_aim(true);
    if (b->get_auto_aim()== true) {
		std::cout << "dijkstra item shoots in if" << std::endl;
        for (auto e : enemies) {
            float distance = glm::distance(e->get_pos(), b->get_pos());
            if (distance < smallest_distance) {
                smallest_distance = distance;
                b->set_autoaim_target(e);
            }
        }
    }

}
