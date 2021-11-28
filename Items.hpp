#pragma once
#include <glm/glm.hpp>

#include <memory>

#include "Player.hpp"
#include "Sprites.hpp"
#include "Bullet.hpp"
#include "GameState.hpp"

class Items {
public:
	Items(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, std::shared_ptr<GameState> gs);
	virtual void preupdate() {};
	virtual void postupdate() {};
	virtual void on_recv_damage() {};
	virtual void on_dealt_damage() {};
	virtual void on_kill() {};
	virtual void on_bullet_destroyed() {};
	virtual void on_shoot(Bullet* b) {};

	glm::vec2 get_pos();
	float get_width();
	Sprite* get_sprite();

protected:
	glm::vec2 pos;
	float width = 32.0f;
	std::shared_ptr<Player> player;
	std::shared_ptr<GameState> gs;
	Sprite* sprite;
};
