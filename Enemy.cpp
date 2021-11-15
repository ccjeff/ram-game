#include "Enemy.hpp"

const glm::vec2& Enemy::get_pos() {
	return this->position;
}

const glm::vec2& Enemy::get_vel() {
	return this->velocity;
}

float Enemy::get_hp() {
	return this->hp;
}

float Enemy::distance(const glm::vec2 &player_pos) {
	return glm::distance(this->position, player_pos);
}