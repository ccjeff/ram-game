#include "Items.hpp"

Items::Items(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite) {
	this->player = player;
	this->pos = pos;
	this->sprite = sprite;
};

glm::vec2 Items::get_pos() {
	return this->pos;
}

float Items::get_width() {
	return this->width;
}

Sprite* Items::get_sprite() {
	return this->sprite;
}