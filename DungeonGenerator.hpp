#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <algorithm>

struct DungeonGenerator;
struct Map;

struct Room {
	size_t x;
	size_t y;
	size_t width;
	size_t height;

	Room(size_t x, size_t y, size_t width, size_t height);

	bool Collides(Room other);

	void Write(Map* map);

	glm::ivec2 GetCenter();

	bool connected;
};

struct Map {
	Map(size_t x, size_t y);

	size_t dimX;
	size_t dimY;

	float scalingFactor = 1;

	std::vector<Room> rooms;
	std::vector<std::vector<int>> map;

	void SetAt(size_t x, size_t y, int value);
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

	glm::ivec2 playerStart;
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

