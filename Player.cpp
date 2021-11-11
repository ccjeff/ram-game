#include "Player.hpp"
#include <iostream>
#include <cmath>

using namespace std;

void Player::move(float elapsed) {
    this->position.x += this->velocity.x * elapsed;
    this->position.y += this->velocity.y * elapsed;
}

void Player::move(glm::vec2 direction, float elapsed) {
    this->velocity.x *= direction.x;
    this->velocity.y *= direction.y;
    move(elapsed);
}

void Player::update(float elapsed, Map &map) {
    glm::vec2 old_pos = this->position;
    move(elapsed);
    if (map.ValueAtWorld(this->position.x, this->position.y) == 0) {
        // this->position = old_pos;
        if (map.ValueAtWorld(old_pos.x, this->position.y) != 0) {
            this->position.x = old_pos.x;
        } else if (map.ValueAtWorld(this->position.x, old_pos.y) != 0) {
            this->position.y = old_pos.y;
        } else {
            this->position = old_pos;
        }
    }

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

void Player::on_hit(float damage) {
	this->hp -= damage;
	if(hp <= 0) {
		//TODO: pull this into a reset function

		cout << "Player died" << endl;
		this->position = glm::vec2(0, 0);
		this->velocity = glm::vec2(0, 0);

		//hp = 5.0f;
	}
}

float Player::get_hp() {
    return this->hp;
}