#pragma once

#include "Bullet.hpp"
#include "Gun.hpp"

#include "GL.hpp"

#include <glm/glm.hpp>

class Pistol: public Gun {
public:
	Pistol();
	Bullet* do_shoot(glm::vec2 pos, glm::vec2 vel) override;

protected:
	float damage = 1.0f;
};