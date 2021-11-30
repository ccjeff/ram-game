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
    if (map.Collides(this->position.x, this->position.y)) {
        // this->position = old_pos;
        if (!map.Collides(old_pos.x, this->position.y)) {
            this->position.x = old_pos.x;
        } else if (!map.Collides(this->position.x, old_pos.y)) {
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

void Player::add_hp(float hp) {
	this->hp += hp;
}

void Player::update_status(float elapsed, PlayerStatus new_status) {
    if((status == IDLE && new_status != IDLE) || new_status == SHOOTING) anim_time_remaining = 0.f;
    anim_time_remaining -= elapsed;
    static float run_duration = *(player_sprites->sprites.at("run").durations.rbegin());
    if(anim_time_remaining <= 0.f) {
        float anim_duration = 0.f;
        if(new_status == IDLE)
            anim_duration = *(player_sprites->sprites.at("idle").durations.rbegin());
        if(new_status == RUNNING)
            anim_duration = *(player_sprites->sprites.at("run").durations.rbegin());
        if(new_status == SHOOTING)
            anim_duration = *(player_sprites->sprites.at("shoot").durations.rbegin());
        anim_time_remaining += ceilf(-anim_time_remaining / anim_duration) * anim_duration;
        status = new_status;
    }
    else if (status == RUNNING && anim_time_remaining <= run_duration / 2.f && new_status != RUNNING) {
        anim_time_remaining -= run_duration / 2.f;
        update_status(0.f, new_status);
    }
}

void Player::draw(std::vector<Vertex> &vertices) {
    Animation anim;
    if(status == IDLE) {
        anim = player_sprites->sprites.at("idle");
    }
    if(status == RUNNING) {
        anim = player_sprites->sprites.at("run");
        face_right = (velocity.x >= 0);
    }
    if (status == SHOOTING) {
        anim = player_sprites->sprites.at("shoot");
    }
    float elapsed = *(anim.durations.rbegin()) - anim_time_remaining;
    anim.draw(elapsed,
            position,
            position,
            glm::vec2(face_right ? width : -width, width),
            0.f,
            glm::u8vec4(255,255,255,255),
            vertices);
}