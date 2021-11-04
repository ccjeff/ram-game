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