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

void Enemy::draw(glm::vec2 camera_center, glm::vec2 size, std::vector<Vertex> &vertices) {
	if(moving) {
		//TODO: fix elapsed calculations
		float anim_duration = *(anim.durations.rbegin());
		anim_elapsed -= floorf(anim_elapsed / anim_duration) * anim_duration;
		anim.draw(anim_elapsed, camera_center, position, size, 0.f, color, vertices);
	}
	else {
		s->draw(camera_center, position, size, 0, color, vertices);
	}
}
