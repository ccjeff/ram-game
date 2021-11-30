#include "GameState.hpp"
#include "Items.hpp"
#include "Enemy.hpp"

GameState::GameState(size_t difficulty_level) {
	// initializing player and dungeon
	dg = new DungeonGenerator(100, 100);
	dg->Generate(20);
	dg->map.SetScalingFactor(64.0f);
	this->difficulty_level = difficulty_level;
	player = std::make_shared<Player>(dg->map.GetWorldCoord(dg->player_start), glm::vec2(0.0f, 0.0f), 32.0f);
}

GameState::~GameState() {
	for(auto e : enemies) {
		delete e;
	}

	for(auto b : bullets) {
		delete b;
	}

	for(auto b : enemy_bullets) {
		delete b;
	}
}
