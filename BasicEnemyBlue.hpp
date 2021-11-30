#pragma once
#include "Enemy.hpp"
#include "Bullet.hpp"

class BasicEnemyBlue : public Enemy {
public:
    ~BasicEnemyBlue() = default;
	BasicEnemyBlue();
    BasicEnemyBlue(glm::vec2 position, glm::vec2 velocity, Sprite* s);
    void move(float elapsed, const glm::vec2 &player_pos, Map &map) override;
    void update(float elapsed) override;
    Bullet* do_attack(const glm::vec2 &player_pos) override;
	void on_hit(Bullet* b) override;

protected:

};