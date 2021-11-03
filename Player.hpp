#pragma once
#include <glm/glm.hpp>
#include "Sprites.hpp"

class Player {
public:
    ~Player() = default;
	Player();
    Player(glm::vec2 position, glm::vec2 velocity) : position(position), velocity(velocity) {};
    void move(float elapsed);
    void move(glm::vec2 direction, float elapsed);
    void update(float elapsed);

protected:
	glm::vec2 position; // should init to a location
    glm::vec2 velocity;
    Sprite s;
};