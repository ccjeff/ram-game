#include "Dijkstra.hpp"
#include "GameState.hpp"
#include <iostream>

using namespace std;

Dijkstra::Dijkstra(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs)
    : Items(player, pos, sprite, gs) {};

void Dijkstra::on_shoot(Bullet *b) {
    float smallest_distance = 99999.0f;

	if(gs->active_room != nullptr) {
		for (auto e : gs->enemies) {

			if(gs->active_room->is_inside(e->get_pos())) {
				float distance = glm::distance(e->get_pos(), b->get_pos());
				if (distance < smallest_distance) {
					smallest_distance = distance;
					b->set_autoaim_target(e);
				}

				b->set_auto_aim(true);
			}
		}
	}
}
