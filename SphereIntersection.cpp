#include "SphereIntersection.hpp"
#include "GameState.hpp"

using namespace std;

void BulletExplosion::draw(glm::vec2 camera_center, vector<Vertex> &vertices){
	static Animation anim = bullet_sprites->sprites.at("aoe_bullet_hit");
	anim.draw(elapsed, camera_center, location, glm::vec2(2 * radius), 0, glm::u8vec4(255,255,255,255), vertices);
}

SphereIntersection::SphereIntersection(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs)
    : Items(player, pos, sprite, gs) {};

void SphereIntersection::on_bullet_destroyed(Bullet* b) {
    for(auto e : gs->enemies) {
		if(e->distance(b->get_pos()) <= gs->dg->map.scalingFactor) {
			e->on_hit(b);
		}
	}
	gs->explosions.push_back({b->get_pos(), 0.f, gs->dg->map.scalingFactor});
}
