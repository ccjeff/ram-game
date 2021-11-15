#pragma once
#include "Enemy.hpp"
#include "Bullet.hpp"

class BasicEnemy : public Enemy {
public:
    ~BasicEnemy() = default;
	BasicEnemy();
    BasicEnemy(glm::vec2 position, glm::vec2 velocity)
		: Enemy(position, velocity) {
		this->hp = 3.0f;
	};
    void move(float elapsed, const glm::vec2 &player_pos, Map &map) override;
    void update(float elapsed) override;
    Bullet* do_attack(const glm::vec2 &player_pos) override;

protected:

};