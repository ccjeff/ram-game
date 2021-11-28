#pragma once
#include "Items.hpp"

class Multithreading : public Items {
public:
    Multithreading(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, std::shared_ptr<GameState> gs);

    void on_shoot(Bullet *b) override;
};