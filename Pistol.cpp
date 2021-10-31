#include "Pistol.hpp"

Pistol::Pistol() {
	this->cooldown = 1.0f;
	this->spread = 0.0f;
}

Bullet* Pistol::do_shoot(glm::vec2 pos, glm::vec2 vel) {
	return new Bullet(pos, vel);
}