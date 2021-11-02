#pragma once
#include "Bullet.hpp"

class Gun {
public:
	virtual Bullet* do_shoot(glm::vec2 pos, glm::vec2 vel) = 0;

protected:
	float cooldown;
	float spread;
};