#pragma once
#include <glm/glm.hpp>
#include "Sprites.hpp"

class Player {
public:
    ~Player() = default;
	Player();
    Player(glm::vec2 position, glm::vec2 velocity, float width) : position(position), velocity(velocity), width(width) {};

    void move(float elapsed);
    void move(glm::vec2 direction, float elapsed);
    void update(float elapsed);

	const glm::vec2& get_pos();
	float get_width();

protected:
	glm::vec2 position; // should init to a location
    glm::vec2 velocity;
    Sprite s;

	float width;
};