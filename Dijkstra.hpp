#pragma once
#include <glm/glm.hpp>
#include "Items.hpp"

class Dijkstra : public Items {
public:
    Dijkstra(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, std::shared_ptr<GameState> gs);

    void on_shoot(Bullet *b) override;
};