#pragma once

#include <glm/glm.hpp>
#include "Sprites.hpp"
#include "Player.hpp"
#include "Bullet.hpp"

#include <iostream>

class GameState;

class Enemy {
public:
    virtual ~Enemy() = default;

    Enemy(glm::vec2 position, glm::vec2 velocity, Sprite *s, GameState* gs)
		: position(position), velocity(velocity), s(s), gs(gs) {};

    virtual void update(float elapsed) {};
    virtual void move(float elapsed, const glm::vec2 &player_pos, Map &map) {};
    virtual Bullet* do_attack(const glm::vec2 &player_pos) { return nullptr; };
	virtual void on_hit(Bullet* b);

    const glm::vec2& get_pos();
    const glm::vec2& get_vel();
    const glm::u8vec4& get_color();

    void set_pos(glm::vec2 &pos);
    void set_vel(glm::vec2 &vel);
    void set_vel(float x, float y);
    void add_vel(float x, float y);

	float get_hp();
    float distance(const glm::vec2 &player_pos);
	float get_width();
	Sprite* get_sprite();
    void set_boss();

protected:
	glm::vec2 position; // should init to a location
    glm::vec2 velocity;
    glm::u8vec4 color = glm::u8vec4(255,255,255,255);
    float speed_val = 40.0f;
    Sprite* s;
	GameState* gs;

	//For determining enemy actions
	float time_step;

	float hp;

	float width = 32.0f;
    bool is_boss = false;
};