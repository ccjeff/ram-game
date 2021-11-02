#include "Player.hpp"

void Player::move(float elapsed) {
    this->position.x += this->velocity.x * elapsed;
    this->position.y += this->velocity.y * elapsed;
}

void Player::move(glm::vec2 direction, float elapsed) {
    this->velocity.x *= direction.x;
    this->velocity.y *= direction.y;
    move(elapsed);
}