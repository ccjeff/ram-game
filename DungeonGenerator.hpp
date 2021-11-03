#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <algorithm>

struct Room;

struct DungeonGenerator {
	int* map;
	size_t dimX;
	size_t dimY;

	glm::ivec2 playerStart;

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

	bool Generate(size_t numberOfRooms);

	void PrintMap();
};

struct Room {
	size_t x;
	size_t y;
	size_t width;
	size_t height;

	Room(size_t x, size_t y, size_t width, size_t height);

	bool Collides(Room other);

	void Write(DungeonGenerator *gen);

	glm::ivec2 GetCenter();
};