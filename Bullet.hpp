#pragma once

#include "GL.hpp"
#include <glm/glm.hpp>
#include <set>

class Enemy;

enum RGB {Red, Green, Blue};

class Bullet {
public:
	Bullet(glm::vec2 pos, glm::vec2 vel);
	void update_pos(float elapsed);
	
	const glm::vec2& get_pos();
	const glm::vec2& get_vel();
	void set_vel(const glm::vec2& vel);

	float get_damage();
	void set_damage(float damage);
	float get_width();
	void set_bounces(int bounces);
	int get_bounces();
	void set_auto_aim(bool auto_aim);
	bool get_auto_aim();
	Enemy* get_autoaim_target();
	void set_autoaim_target(Enemy* target);
	
protected:
	glm::vec2 pos;
	glm::vec2 vel;

	float damage = 1.0f;
	float width = 16.0f;
	int bounces = 0;
	bool auto_aim = false;
	float auto_aim_timer = 0.0f;
	Enemy* autoaim_target = nullptr;

	std::set<RGB> rgb;
};