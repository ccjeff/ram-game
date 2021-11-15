#pragma once
#include <glm/glm.hpp>

#include <memory>

#include "Player.hpp"
#include "Sprites.hpp"

class Items {
public:
	Items(std::shared_ptr<Player> player, glm::vec2 pos);
	virtual void preupdate() {};
	virtual void postupdate() {};
	virtual void on_recv_damage() {};
	virtual void on_dealt_damage() {};
	virtual void on_kill() {};
	virtual void on_bullet_destroyed() {};

	glm::vec2 get_pos();
	float get_width();

protected:
	glm::vec2 pos;
	float width;
	std::shared_ptr<Player> player;
};
