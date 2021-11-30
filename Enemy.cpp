#include "Enemy.hpp"
#include "Items.hpp"
#include "Dijkstra.hpp"
#include <iostream>

const glm::vec2& Enemy::get_pos() {
	return this->position;
}

const glm::vec2& Enemy::get_vel() {
	return this->velocity;
}

const glm::u8vec4& Enemy::get_color() {
	return this->color;
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

void Enemy::on_hit(Bullet* b) {
	this->hp -= b->get_damage();
}

void Enemy::set_boss() {
	this->is_boss = true;
}