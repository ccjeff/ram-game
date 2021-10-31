#include "Bullet.hpp"

Bullet::Bullet(glm::vec2 pos, glm::vec2 vel) : pos(pos), vel(vel) {

}

void Bullet::update_pos(float elapsed) {
	this->pos += this->vel * elapsed;
}

glm::vec2 Bullet::get_pos() {
	return this->pos;
}