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

void Player::update(float elapsed, int *map, size_t x, size_t y) {
    glm::vec2 old_pos = this->position;
    move(elapsed);
    if (this->position.x < 0 || this->position.x > x - 1 || this->position.y < 0 || this->position.y > y - 1 ||
        (map[(int)this->position.x * y + (int)this->position.y] != 0)) {
        this->position = old_pos;
    }

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
