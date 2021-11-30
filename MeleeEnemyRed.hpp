#pragma once
#include "Enemy.hpp"
#include "Bullet.hpp"

class MeleeEnemyRed : public Enemy {
public:
    ~MeleeEnemyRed() = default;
	MeleeEnemyRed();
    MeleeEnemyRed(glm::vec2 position, glm::vec2 velocity, Sprite* s)
		: Enemy(position, velocity, s) {
		this->time_step = 5.0f;
		this->hp = 3.0f;
		this->color = glm::u8vec4(255,100,100,255);
	};
    void move(float elapsed, const glm::vec2 &player_pos, Map &map) override;
    void update(float elapsed) override;
    Bullet* do_attack(const glm::vec2 &player_pos) override;
	void on_hit(Bullet* b) override;

protected:

};