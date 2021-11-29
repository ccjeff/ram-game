#pragma once

#include <glm/glm.hpp>
#include "Sprites.hpp"
#include "Player.hpp"
#include "Bullet.hpp"

class Enemy {
public:
    virtual ~Enemy() = default;

    Enemy(glm::vec2 position, glm::vec2 velocity, Sprite *s)
		: position(position), velocity(velocity), s(s){};

    virtual void update(float elapsed) {};
    virtual void move(float elapsed, const glm::vec2 &player_pos, Map &map) {};
    virtual Bullet* do_attack(const glm::vec2 &player_pos) { return nullptr; };
	virtual void on_hit(float damage);

    const glm::vec2& get_pos();
    const glm::vec2& get_vel();

    void set_pos(glm::vec2 &pos);
    void set_vel(glm::vec2 &vel);
    void set_vel(float x, float y);
    void add_vel(float x, float y);

	float get_hp();
    float distance(const glm::vec2 &player_pos);
	float get_width();
	Sprite* get_sprite();

protected:
	glm::vec2 position; // should init to a location
    glm::vec2 velocity;
    float speed_val = 40.0f;
    Sprite* s;

	//For determining enemy actions
	float time_step;

	float hp;

	float width = 32.0f;
};