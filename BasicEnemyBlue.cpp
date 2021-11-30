#include "BasicEnemyBlue.hpp"
#include "GameState.hpp"

using namespace std;

BasicEnemyBlue::BasicEnemyBlue(glm::vec2 position, glm::vec2 velocity, Sprite* s, GameState* gs)
		: Enemy(position, velocity, s, gs) {
		this->time_step = float((rand() % 1000)) / 200.0f;
		this->hp = 3.0f;
		this->color = glm::u8vec4(0,100,200,255);
}

void BasicEnemyBlue::update(float elapsed) {
	// cout << time_step << endl;
	time_step += elapsed;
}

void BasicEnemyBlue::move(float elapsed, const glm::vec2 &player_pos, Map &map) {
	glm::vec2 old_pos = this->position;
	if (distance(player_pos) > 3.0f * this->get_width()) {
		glm::vec2 direction = glm::normalize(player_pos - this->position);
		this->set_vel(direction);
		this->position += direction * speed_val * elapsed;
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
	}
	//std::cout << "BasicEnemyBlue::move" << this->position.x << "&" << this->position.y << std::endl;
}


Bullet* BasicEnemyBlue::do_attack(const glm::vec2 &player_pos) {
	if(time_step > 5.0f) {
		Bullet* b = new Bullet(this->position, 
			glm::normalize(player_pos - this->position + 1.0f)
		);
		//TODO: update bullet damage
		b->set_damage(1.0f);
		// cout <<player_pos.x << " " << player_pos.y << endl;
		// cout << this->position.x << " " << this->position.y << endl;
		// cout << b->get_vel().x << " " << b->get_vel().y << endl;
		time_step = 0.0f;
		return b;
	}
	
	return nullptr;
}

void BasicEnemyBlue::on_hit(Bullet* b) {
	if(b->get_rgb().find(RGB::Blue) != b->get_rgb().end()) {
		this->hp -= b->get_damage();
	}
	else {
		this->hp -= 0.5f * b->get_damage();
	}
}