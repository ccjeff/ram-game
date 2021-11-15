#pragma once

#include <memory>
#include "Player.hpp"

class Items {
public:
	Items(std::shared_ptr<Player> player);
	virtual void preupdate() {};
	virtual void postupdate() {};
	virtual void on_recv_damage() {};
	virtual void on_dealt_damage() {};
	virtual void on_kill() {};
	virtual void on_bullet_destroyed() {};

protected:
	std::shared_ptr<Player> player;
};
