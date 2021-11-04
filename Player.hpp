#pragma once
#include <glm/glm.hpp>
#include "Sprites.hpp"
	
#define PLAYER_SPEED 100.0f
#define PLAYER_MAX_SPEED 250.0f

class Player {
public:
    ~Player() = default;
	Player();
    Player(glm::vec2 position, glm::vec2 velocity, float width) : position(position), velocity(velocity), width(width) {};

    void move(float elapsed);
    void move(glm::vec2 direction, float elapsed);
    void update(float elapsed);

	float get_width();
	
    const glm::vec2& get_pos();
    const glm::vec2& get_vel();

    void set_pos(glm::vec2 &pos);
    void set_vel(const glm::vec2 &vel);
    void set_vel(float x, float y);
    void add_vel(float x, float y);

protected:
	glm::vec2 position; // should init to a location
    glm::vec2 velocity;
    Sprite s;

	float width;
};