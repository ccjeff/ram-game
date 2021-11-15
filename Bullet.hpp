#pragma once

#include "GL.hpp"

#include <glm/glm.hpp>

class Bullet {
public:
	Bullet(glm::vec2 pos, glm::vec2 vel);
	void update_pos(float elapsed);
	
	const glm::vec2& get_pos();
	const glm::vec2& get_vel();

	float get_damage();
	float get_width();
	
protected:
	glm::vec2 pos;
	glm::vec2 vel;

	float damage = 1.0f;
	float width = 16.0f;
};