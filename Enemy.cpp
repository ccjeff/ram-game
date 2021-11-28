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
