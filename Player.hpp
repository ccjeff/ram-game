#pragma once
#include <glm/glm.hpp>
#include "Sprites.hpp"

class Player {
public:
	Player() = default;
    ~Player() = default;
    Player(glm::vec2 position, glm::vec2 rotation) : position(position), rotation(rotation) {};
    void move(float elapsed);
    void move(glm::vec2 direction, float elapsed);
    void update(float elapsed);

protected:
	glm::vec2 position; // should init to a location
    glm::vec2 velocity;
    glm::vec2 acceleration;
    Sprite s;

};