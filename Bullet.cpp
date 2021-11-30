#include "Bullet.hpp"

Bullet::Bullet(glm::vec2 pos, glm::vec2 vel) : pos(pos), vel(vel) {}

void Bullet::update_pos(float elapsed) {
	this->pos += this->vel * elapsed;
}

const glm::vec2& Bullet::get_pos() {
	return this->pos;
}

const glm::vec2& Bullet::get_vel() {
	return this->vel;
}

void Bullet::set_vel(const glm::vec2& vel) {
	this->vel = vel;
}

float Bullet::get_damage() {
	return this->damage;
}

void Bullet::set_damage(float damage) {
	this->damage = damage;
}

float Bullet::get_width() {
	return this->width;
}

void Bullet::set_bounces(int bounces) {
	this->bounces = bounces;
}

void Bullet::set_auto_aim(bool auto_aim) {
	this->auto_aim = auto_aim;
}

bool Bullet::get_auto_aim() {
	return this->auto_aim;
}

int Bullet::get_bounces() {
	return this->bounces;
}

Enemy* Bullet::get_autoaim_target() {
	return this->autoaim_target;
}

void Bullet::set_autoaim_target(Enemy* target) {
	this->autoaim_target = target;
}

std::set<RGB>& Bullet::get_rgb() {
	return this->rgb;
}