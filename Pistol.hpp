#pragma once

#include "Bullet.hpp"
#include "Gun.cpp"

#include "GL.hpp"

#include <glm/glm.hpp>

class Pistol: public Gun {
public:
	Pistol();
	Bullet* do_shoot(glm::vec2 pos, glm::vec2 vel) override;
};