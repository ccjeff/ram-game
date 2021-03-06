#pragma once
#include <vector>

#include "Bullet.hpp"
#include "Pistol.hpp"
#include "Gun.hpp"
#include "Player.hpp"

//Enemies
#include "BasicEnemyRed.hpp"
#include "BasicEnemyGreen.hpp"
#include "BasicEnemyBlue.hpp"
#include "MeleeEnemyRed.hpp"
#include "MeleeEnemyGreen.hpp"
#include "MeleeEnemyBlue.hpp"

//Items
#include "ReinforcementLearning.hpp"
#include "RayTracing.hpp"
#include "Dijkstra.hpp"
#include "P_NP.hpp"
#include "Multithreading.hpp"
#include "SphereIntersection.hpp"
#include "RNG.hpp"
#include "RubberDucky.hpp"
#include "Debugger.hpp"
#include "ThermalPaste.hpp"

#include "DungeonGenerator.hpp"

class Items;

class Enemy;

class GameState {
public:
	GameState(size_t difficulty_level);
	~GameState();

	DungeonGenerator* dg;
	Room* active_room = nullptr;

	size_t difficulty_level = 1;

	std::vector<Bullet*> bullets;
	std::vector<BulletExplosion> explosions;

	std::vector<Bullet*> enemy_bullets;
	std::vector<Enemy*> enemies;

	std::shared_ptr<Player> player;

	std::set<Items*> item_set;
	int num_items;

	std::vector<Items*> items;
	std::vector<Items*> items_on_ground;
};