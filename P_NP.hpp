#pragma once
#include <glm/glm.hpp>
#include "Items.hpp"

class P_NP : public Items {
public:
    P_NP(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite);

    void on_shoot(Bullet *b, std::vector<Enemy*>& enemies) override;
};