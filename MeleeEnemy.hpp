#pragma once
#include "Enemy.hpp"
#include "Bullet.hpp"

class MeleeEnemy : public Enemy {
public:
    ~MeleeEnemy() = default;
	MeleeEnemy();
    MeleeEnemy(glm::vec2 position, glm::vec2 velocity, Sprite* s)
		: Enemy(position, velocity, s) {
		this->time_step = 5.0f;
		this->hp = 3.0f;
	};
    void move(float elapsed, const glm::vec2 &player_pos, Map &map) override;
    void update(float elapsed) override;
    Bullet* do_attack(const glm::vec2 &player_pos) override;

protected:

};