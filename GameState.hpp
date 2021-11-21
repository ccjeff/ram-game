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
#include "Items.hpp"
#include "ReinforcementLearning.hpp"
#include "RayTracing.hpp"
#include "DungeonGenerator.hpp"

struct GameState {
	GameState();
	~GameState();

	DungeonGenerator* dg;
	Room* active_room = NULL;

	std::vector<Bullet*> bullets;

	std::vector<Bullet*> enemy_bullets;
	std::vector<Enemy*> enemies;

	std::shared_ptr<Player> player;

	std::vector<Items*> items;
	std::vector<Items*> items_on_ground;
};