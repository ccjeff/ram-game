#include "Items.hpp"

Items::Items(std::shared_ptr<Player> player, glm::vec2 pos) {
	this->player = player;
	this->pos = pos;
};

glm::vec2 Items::get_pos() {
	return this->pos;
}

float Items::get_width() {
	return this->width;
}