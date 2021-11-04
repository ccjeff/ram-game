#pragma once
#include <glm/glm.hpp>
#include "Sprites.hpp"
#include "Player.hpp"
#include "Bullet.hpp"

class Enemy {
public:
    Enemy(glm::vec2 position, glm::vec2 velocity)
		: position(position), velocity(velocity){};

    virtual void update(float elapsed) {};
    virtual Bullet* do_attack(const glm::vec2 &player_pos) { return nullptr; };

    const glm::vec2& get_pos();
    const glm::vec2& get_vel();

    void set_pos(glm::vec2 &pos);
    void set_vel(glm::vec2 &vel);
    void set_vel(float x, float y);
    void add_vel(float x, float y);

protected:
	glm::vec2 position; // should init to a location
    glm::vec2 velocity;
    Sprite s;

	//For determining enemy actions
	float time_step;
};