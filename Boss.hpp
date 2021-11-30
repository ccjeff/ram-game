#pragma once

#include "Enemy.hpp"

class Boss : public Enemy {
public:
    ~Boss() = default;
    Boss();
    Boss(glm::vec2 position, glm::vec2 velocity, Sprite* s);
    void move(float elapsed, const glm::vec2 &player_pos, Map &map) override;
    void update(float elapsed) override;
    Bullet* do_attack(const glm::vec2 &player_pos) override;
    void on_hit(Bullet* b) override;
protected:
    void boss_attack(const glm::vec2 &player_pos, Map &map);
};