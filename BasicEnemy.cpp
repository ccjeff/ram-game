#include "BasicEnemy.hpp"

#include <iostream>

using namespace std;

void BasicEnemy::update(float elapsed) {
	time_step += elapsed;
}

Bullet* BasicEnemy::do_attack(const glm::vec2 &player_pos) {
	if(time_step > 5.0f) {
		Bullet* b = new Bullet(this->position, player_pos - this->position);

		cout <<player_pos.x << " " << player_pos.y << endl;
		cout << this->position.x << " " << this->position.y << endl;

		cout << b->get_vel().x << " " << b->get_vel().y << endl;
		time_step = 0.0f;
		return b;
	}
	
	return nullptr;
}