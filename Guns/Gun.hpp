#pragma once

#include "../Bullet.hpp"

class Gun {
public:
	virtual Bullet do_shoot();

protected:
	float cooldown;
	float spread;
};