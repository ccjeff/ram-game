#pragma once
#include <glm/glm.hpp>
#include "Sprites.hpp"

class Player {
public:
    ~Player() = default;
    Player(glm::vec2 position, glm::vec2 velocity) : position(position), velocity(velocity) {};
    void move(float elapsed);
    void move(glm::vec2 direction, float elapsed);
    void update(float elapsed);
    glm::vec2 get_pos();
    glm::vec2 get_vel();
    void set_pos(glm::vec2 pos);
    void set_vel(glm::vec2 vel);
    void set_vel(float x, float y);
    void add_vel(float x, float y);
protected:
	glm::vec2 position; // should init to a location
    glm::vec2 velocity;
    // Sprite s;
};