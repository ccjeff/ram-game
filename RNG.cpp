#include "RNG.hpp"
#include "GameState.hpp"

using namespace std;

RNG::RNG(std::shared_ptr<Player> player, glm::vec2 pos, Sprite* sprite, GameState* gs)
    : Items(player, pos, sprite, gs) {};

void RNG::on_shoot(Bullet* b) {
    int num = rand() % 10;

	//Expectation: 1/10 * 10 + 2/10 * 0 + 7/10 * 1 = 1.7
	if(num == 0) {
		b->set_damage(b->get_damage() + 10.0f);
	}
	else if(num <= 2) {
		b->set_damage(0);
	}

	cout << b->get_damage() << endl;
}