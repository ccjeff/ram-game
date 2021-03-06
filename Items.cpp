#include "Items.hpp"
#include "GameState.hpp"

Items::Items(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs) {
	this->player = player;
    this->gs = gs;
	this->pos = pos;
	this->sprite = sprite;
};

glm::vec2 Items::get_pos() {
	return this->pos;
}

void Items::set_pos(glm::vec2 pos) {
	this->pos = pos;
}

float Items::get_width() {
	return this->width;
}

Sprite* Items::get_sprite() {
	return this->sprite;
}

void Items::set_ui_sprite(Sprite* sprite)
{
	ui_sprite = sprite;
}

Sprite* Items::get_ui_sprite()
{
	return ui_sprite;
}