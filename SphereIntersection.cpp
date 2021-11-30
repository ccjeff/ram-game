#include "SphereIntersection.hpp"
#include "GameState.hpp"

using namespace std;

SphereIntersection::SphereIntersection(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs)
    : Items(player, pos, sprite, gs) {};

void SphereIntersection::on_bullet_destroyed(Bullet* b) {
    for(auto e : gs->enemies) {
		if(e->distance(b->get_pos()) <= gs->dg->map.scalingFactor) {
			e->on_hit(b);
		}
	}
}
