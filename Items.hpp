#pragma once
#include <glm/glm.hpp>

#include <memory>

#include "Player.hpp"
#include "Sprites.hpp"
#include "Bullet.hpp"

struct GameState;

class Items {
public:
	Items(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite);
	virtual void preupdate() {};
	virtual void postupdate() {};
	virtual void on_recv_damage() {};
	virtual void on_dealt_damage() {};
	virtual void on_kill() {};
	virtual void on_bullet_destroyed() {};
	virtual void on_shoot(Bullet* b, std::vector<Enemy*>& enemies) {};

	glm::vec2 get_pos();
	float get_width();
	Sprite* get_sprite();

protected:
	glm::vec2 pos;
	float width = 32.0f;
	std::shared_ptr<Player> player;
	Sprite* sprite;
};
