#include "Bullet.hpp"

Bullet::Bullet(glm::vec2 pos, glm::vec2 vel) : pos(pos), vel(vel) {}

void Bullet::update_pos(float elapsed) {
	this->pos += this->vel * elapsed;
}

const glm::vec2& Bullet::get_pos() {
	return this->pos;
}

const glm::vec2& Bullet::get_vel() {
	return this->vel;
}

void Bullet::set_vel(const glm::vec2& vel) {
	this->vel = vel;
}

float Bullet::get_damage() {
	return this->damage;
}

float Bullet::get_width() {
	return this->width;
}

void Bullet::set_bounces(int bounces) {
	this->bounces = bounces;
}

int Bullet::get_bounces() {
	return this->bounces;
}