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

void Player::update(float elapsed) {
    move(elapsed);
	velocity = velocity * 0.8f;
}

float Player::get_width() {
	return this->width;
}

const glm::vec2& Player::get_pos() {
	return this->position;
}

const glm::vec2& Player::get_vel() {
    return this->velocity;
}

void Player::set_pos(glm::vec2 &pos) {
    this->position = std::move(pos);
}
void Player::set_vel(const glm::vec2 &vel) {
    this->velocity = std::move(vel);
}

void Player::set_vel(float x, float y) {
    this->velocity.x = x;
    this->velocity.y = y;
}

void Player::add_vel(float x, float y) {
    this->velocity.x += x;
    this->velocity.y += y;
}