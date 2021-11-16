#include "MeleeEnemy.hpp"

#include <iostream>

using namespace std;

void MeleeEnemy::update(float elapsed) {
	// cout << time_step << endl;
	time_step += elapsed;
}

void MeleeEnemy::move(float elapsed, const glm::vec2 &player_pos, Map &map) {
	glm::vec2 old_pos = this->position;
    glm::vec2 direction = glm::normalize(player_pos - this->position);
    this->set_vel(direction);
    this->position += direction * speed_val* 2.0f * elapsed;
    if (map.ValueAtWorld(this->position.x, this->position.y) == 0) {
        // this->position = old_pos;
        if (map.ValueAtWorld(old_pos.x, this->position.y) != 0) {
            this->position.x = old_pos.x;
        } else if (map.ValueAtWorld(this->position.x, old_pos.y) != 0) {
            this->position.y = old_pos.y;
        } else {
            this->position = old_pos;
        }
    }
	//std::cout << "BasicEnemy::move" << this->position.x << "&" << this->position.y << std::endl;
}


Bullet* MeleeEnemy::do_attack(const glm::vec2 &player_pos) {
	if(time_step >= 2.0f && distance(player_pos) < 1.0f) {
		Bullet* b = new Bullet(this->position, 
			glm::normalize(player_pos - this->position + 1.0f)
		);

		time_step = 0.0f;
		return b;
	}
	
	return nullptr;
}