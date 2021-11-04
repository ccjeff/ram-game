#include "BasicEnemy.hpp"

void BasicEnemy::update(float elapsed) {
	time_step += elapsed;
}

Bullet* BasicEnemy::do_attack(const glm::vec2 &player_pos) {
	if(time_step > 5.0f) {
		Bullet* b = new Bullet(this->position, player_pos - this->position);
		time_step = 0.0f;
		return b;
	}
	
	return nullptr;
}