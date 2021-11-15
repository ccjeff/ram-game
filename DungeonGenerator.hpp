#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <algorithm>
#include <string.h>
#include "data_path.hpp"

#define doorNum 2

struct DungeonGenerator;
struct Map;



struct RoomTemplate
{
	size_t width;
	size_t height;
	std::vector<std::vector<int>> layout;

	RoomTemplate(std::string path);
};

struct Room {
	size_t x;
	size_t y;
	size_t width;
	size_t height;
	std::vector<std::vector<int>> layout;

	Map* map;

	Room(size_t width, size_t height);
	Room(RoomTemplate roomTemplate);

	void SetPosition(size_t x, size_t y);

	bool is_inside(glm::vec2 position);

	bool Collides(Room other);

	void SetMap(Map* map);

	void Write();

	glm::ivec2 GetCenter();

	void LockRoom();
	void UnlockRoom();

	bool connected;
};

struct Map {
	Map(size_t x, size_t y);

	size_t dimX;
	size_t dimY;

	float scalingFactor = 1;

	std::vector<Room> rooms;
	std::vector<std::vector<int>> map;
	std::vector<std::vector<bool>> collision;
	std::vector<std::vector<int>> spriteMap;

	void SetAt(size_t x, size_t y, int value);
	void SetCollisionAt(size_t x, size_t y, bool value);
	int ValueAt(size_t x, size_t y);

	int ValueAtWorld(float x, float y);

	glm::vec2 Boundary();

	glm::vec2 GetWorldCoord(glm::ivec2 mapCoord);
	glm::vec2 GetWorldCoord(int x, int y);

	glm::ivec2 GetTile(float x, float y);

	void SetScalingFactor(float factor);
};

struct DungeonGenerator {
	Map map = Map(0, 0);
	size_t dimX;
	size_t dimY;

	std::vector<std::vector<int>> tiles;

	glm::ivec2 player_start;
	std::vector<glm::vec2> monsterPositions;

	//-----------Variables to use while rooms are generated-------------

	//Room Dimensions
	size_t minRoomSize = 5;
	size_t maxRoomSize = 10;

	size_t attemptsPerRoom = 100;


	DungeonGenerator(size_t x, size_t y);
	~DungeonGenerator();

	std::vector<Room> rooms;

	size_t index(size_t xIndex, size_t yIndex);

	void SetAt(size_t x, size_t y, int value);

	void DrawCorridor(Room one, Room two);

	void ConnectRooms();

	bool CompareDistToStart(Room a, Room b);

	bool Generate(size_t numberOfRooms);

	void PrintMap();
};

