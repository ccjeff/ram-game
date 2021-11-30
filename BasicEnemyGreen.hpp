#pragma once
#include "Enemy.hpp"
#include "Bullet.hpp"

class BasicEnemyGreen : public Enemy {
public:
    ~BasicEnemyGreen() = default;
	BasicEnemyGreen();
    BasicEnemyGreen(glm::vec2 position, glm::vec2 velocity, Sprite* s, GameState* gs);
    void move(float elapsed, const glm::vec2 &player_pos, Map &map) override;
    void update(float elapsed) override;
    Bullet* do_attack(const glm::vec2 &player_pos) override;
	void on_hit(Bullet* b) override;

protected:

};