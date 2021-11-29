#pragma once
#include <vector>

#include "Bullet.hpp"
#include "Pistol.hpp"
#include "Gun.hpp"
#include "Player.hpp"

//Enemies
#include "Enemy.hpp"
#include "BasicEnemy.hpp"
#include "MeleeEnemy.hpp"

//Items
#include "ReinforcementLearning.hpp"
#include "RayTracing.hpp"
#include "Dijkstra.hpp"
#include "P_NP.hpp"
#include "Multithreading.hpp"

#include "DungeonGenerator.hpp"

class Items;

class GameState {
public:
	GameState();
	~GameState();

	DungeonGenerator* dg;
	Room* active_room = nullptr;

	std::vector<Bullet*> bullets;

	std::vector<Bullet*> enemy_bullets;
	std::vector<Enemy*> enemies;

	std::shared_ptr<Player> player;

	std::vector<Items*> items;
	std::vector<Items*> items_on_ground;
};