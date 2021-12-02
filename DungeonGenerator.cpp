#define _CRT_SECURE_NO_DEPRECATE

#include "DungeonGenerator.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>


/*extern "C" {
	FILE	*fopen(const char * __restrict __filename, const char * __restrict __mode);
	int fscanf ( FILE * stream, const char * format, ... );
	int fclose ( FILE * stream );
}*/

using namespace std;

bool collisionMat[] = { true, false, false, true };
bool bulletCollisionMat[] = { true, false, true, true };
int floorDecorations[] = { 4, 5, 6 };
int horizontalDecorations[] = { 7, 8, 9 };
int verticalDecorations[] = { 11, 12, 13 };

std::string prefabs[] = { "test1", "test2", "test3", "test4"};

DungeonGenerator::DungeonGenerator(size_t x, size_t y)
{
	dimX = x;
	dimY = y;

	map = Map(x, y);
}

void DungeonGenerator::SetAt(size_t x, size_t y, int value)
{
	map.SetAt(x, y, value);
}

//So we can safely iterate over (x, y) this is flipped
size_t DungeonGenerator::index(size_t xIndex, size_t yIndex)
{
	return yIndex + dimY * xIndex;
}

bool DungeonGenerator::Generate(size_t numberOfRooms)
{
	rooms.clear();
	for (size_t count = 0; count < numberOfRooms; count++)
	{
		bool success = true;
		for (size_t attempt = 0; attempt < attemptsPerRoom; attempt++)
		{
			int len = sizeof(prefabs) / sizeof(prefabs[0]);
			RoomTemplate temp = RoomTemplate(prefabs[rand() % len]);

			
			size_t width = temp.width;// (rand() % (maxRoomSize - minRoomSize)) + minRoomSize;
			size_t height = temp.height;// (rand() % (maxRoomSize - minRoomSize)) + minRoomSize;
			size_t startX = (rand() % (dimX - width  - 2)) + 1;
			size_t startY = (rand() % (dimY - height - 2)) + 1;

			
			Room newRoom = Room(temp);
			newRoom.SetPosition(startX, startY);

			success = true;
			for (Room r : rooms)
			{
				success = !r.Collides(newRoom);
				if (!success)
				{
					break;
				}
			}

			if (success)
			{
				rooms.push_back(newRoom);
				break;
			}
		}
		if (!success)
		{
			return false;
		}
	}

	for (Room r : rooms)
	{
		r.SetMap(&map);
		r.Write();
		map.rooms.push_back(r);
	}


	ConnectRooms();

	for (Room r : rooms)
	{
		r.SetMap(&map);
		r.WriteDoors();
	}


	size_t playerRoom = rand() % rooms.size();
	player_start = rooms[playerRoom].GetCenter();

	for (size_t i = 0; i < rooms.size(); i++)
	{
		rooms[i].SetMap(&map);
		if (i == playerRoom) continue;
		
		std::vector<glm::vec2> positions = rooms[i].CreatePositions(5);
		for (size_t i = 0; i < positions.size(); i++)
		{
			monsterPositions.push_back(positions[i]);
		}
	}

	AddDecorations();

	PrintMap();

	return true;
}

bool CompareDist(Room a, Room b)
{
	return glm::length((glm::vec2) a.GetCenter()) < glm::length((glm::vec2) b.GetCenter());
}

void DungeonGenerator::ConnectRooms()
{
	std::vector<Room> connected;

	std::sort(rooms.begin(), rooms.end(),
		CompareDist);

	rooms[0].connected = true;
	connected.push_back(rooms[0]);

	for (size_t i = 1; i < rooms.size(); i++)
	{
		//printf("Connecting room %d\n", i);
		float best = 1000000.f;
		Room bestRoom = connected[0];
		for (Room r : connected)
		{
			float dist = glm::length((glm::vec2)(r.GetCenter() - rooms[i].GetCenter()));
			if (dist < best)
			{
				bestRoom = r;
				best = dist;
			}
		}

		DrawCorridor(rooms[i], bestRoom);
		rooms[i].connected = true;
		connected.push_back(rooms[i]);
	}
}


void DungeonGenerator::DrawCorridor(Room one, Room two)
{
	glm::ivec2 first = one.GetCenter();
	glm::ivec2 second = two.GetCenter();
	if (rand() % 2 == 0)
	{
		//Horizontal corridor
		int halfway = (first.x + second.x) / 2;

		for (int i = std::min(first.x, halfway); i <= std::max(first.x, halfway); i++)
		{
			map.SetAt(i, first.y, 1);
		}

		for (int j = std::min(first.y, second.y); j <= std::max(first.y, second.y); j++)
		{
			map.SetAt(halfway, j, 1);
		}

		for (int i = std::min(second.x, halfway); i <= std::max(second.x, halfway); i++)
		{
			map.SetAt(i, second.y, 1);
		}
	}
	else
	{
		//Vertical corridor
		int halfway = (first.y + second.y) / 2;

		for (int j = std::min(first.y, halfway); j <= std::max(first.y, halfway); j++)
		{
			map.SetAt(first.x, j, 1);
		}

		for (int i = std::min(first.x, second.x); i <= std::max(first.x, second.x); i++)
		{
			map.SetAt(i, halfway, 1);
		}

		for (int j = std::min(second.y, halfway); j <= std::max(second.y, halfway); j++)
		{
			map.SetAt(second.x, j, 1);
		}
	}
}

void DungeonGenerator::AddDecorations()
{
	for (size_t index = 0; index < numberOfDecorations; index++)
	{
		switch (rand() % 3) {
			case 0: //Add tile decorations
				for (size_t attempt = 0; attempt < 100; attempt++)
				{
					size_t x = rand() % map.dimX;
					size_t y = rand() % map.dimY;
					if (map.ValueAt(x, y) != 1) continue;
					map.SetAtNoCollision(x, y, floorDecorations[rand() % (sizeof(floorDecorations) / sizeof(int))]);
					break;
				}
				break;
			case 1: //Add long multi-tile
				for (size_t attempt = 0; attempt < 100; attempt++)
				{
					size_t x = rand() % (map.dimX - 2);
					size_t y = (rand() % (map.dimY - 2)) + 1;
					bool success = true;
					for (size_t i = 0; i < 3; i++)
					{
						if (map.ValueAt(x + i, y) != 0)
						{
							success = false;
							break;
						}
						if (map.ValueAt(x + i, y - 1) != 1 && map.ValueAt(x + i, y + 1) != 1)
						{
							success = false;
							break;
						}
					}
					if (!success) continue;
					map.SetAtNoCollision(x    , y, horizontalDecorations[0]);
					map.SetAtNoCollision(x + 1, y, horizontalDecorations[1]);
					map.SetAtNoCollision(x + 2, y, horizontalDecorations[2]);
					break;
				}
				break;
			case 2: //Add Tall multi-tile
				
				for (size_t attempt = 0; attempt < 100; attempt++)
				{
					size_t x = (rand() % (map.dimX - 2)) + 1;
					size_t y = (rand() % (map.dimY - 2));
					bool success = true;
					for (size_t j = 0; j < 3; j++)
					{
						if (map.ValueAt(x, y + j) != 0)
						{
							success = false;
							break;
						}
						if (map.ValueAt(x - 1, y + j) != 1 && map.ValueAt(x + 1, y + j) != 1)
						{
							success = false;
							break;
						}
					}
					if (!success) continue;
					map.SetAtNoCollision(x, y    , verticalDecorations[0]);
					map.SetAtNoCollision(x, y + 1, verticalDecorations[1]);
					map.SetAtNoCollision(x, y + 2, verticalDecorations[2]);
					break;
				}
				break;
			default:
				break;
		}
	}
	
}

void DungeonGenerator::PrintMap()
{
	for (size_t y = 0; y < dimY; y++)
	{
		for (size_t x = 0; x < dimX; x++)
		{
			//printf("%d", map.ValueAt(x, y));
		}
		//printf(" - %zu\n", y);
	}

	for (size_t y = 0; y < dimY; y++)
	{
		for (size_t x = 0; x < dimX; x++)
		{
			if (map.collision[x][y])
			{
				//printf("X");
			}
			else
			{
				//printf(" ");
			}
		}
		//printf("\n");
	}
}

Map::Map(size_t x, size_t y)
{
	dimX = x;
	dimY = y;

	map.clear();
	rooms.clear();
	collision.clear();
	spriteMap.clear();

	map = std::vector<std::vector<int>> (dimX, std::vector<int>(dimY, 0));
	collision = std::vector<std::vector<bool>>(dimX, std::vector<bool>(dimY, true));
	bulletCollision = std::vector<std::vector<bool>>(dimX, std::vector<bool>(dimY, true));
	spriteMap = std::vector<std::vector<int>>(dimX, std::vector<int>(dimY, 0));
}

void Map::SetAt(size_t x, size_t y, int value)
{
	SetAtNoCollision(x, y, value);
	SetCollisionAt(x, y, collisionMat[value]);
	SetBulletCollisionAt(x, y, bulletCollisionMat[value]);
}

void Map::SetCollisionAt(size_t x, size_t y, bool value)
{
	collision[x][y] = value;
}

void Map::SetBulletCollisionAt(size_t x, size_t y, bool value)
{
	bulletCollision[x][y] = value;
}

void Map::SetAtNoCollision(size_t x, size_t y, int value)
{
	map[x][y] = value;
}

int Map::ValueAt(size_t x, size_t y)
{
	return map[x][y];
}

int Map::ValueAtWorld(float x, float y)
{
	glm::ivec2 coord = GetTile(x, y);
	if (coord.x < 0 || coord.y < 0 || coord.x >= int(dimX) || coord.y >= int(dimY))
	{
		//Outside of bounds is a wall
		return 0;
	}
	return ValueAt(coord.x, coord.y);
}

bool Map::BulletCollides(float x, float y)
{
	glm::ivec2 coord = GetTile(x, y);
	if (coord.x < 0 || coord.y < 0 || coord.x >= int(dimX) || coord.y >= int(dimY))
	{
		//Outside of bounds is a wall
		return true;
	}
	return bulletCollision[coord.x][coord.y];
}

bool Map::Collides(float x, float y)
{
	glm::ivec2 coord = GetTile(x, y);
	if (coord.x < 0 || coord.y < 0 || coord.x >= int(dimX) || coord.y >= int(dimY))
	{
		//Outside of bounds is a wall
		return 0;
	}
	return collision[coord.x][coord.y];
}

glm::ivec2 Map::GetTile(float x, float y)
{
	if (x < 0 || y < 0 || x / scalingFactor >= dimX || y / scalingFactor >= dimY)
	{
		return glm::ivec2(-1, -1);
	}
	return glm::ivec2((int)(x / scalingFactor), (int)(y / scalingFactor));
}

void Map::SetScalingFactor(float factor)
{
	scalingFactor = factor;
}

//Returns the largest possible point that could be considered in the map:
//Check if things are > this to see if they're out of bounds!
glm::vec2 Map::Boundary()
{
	return scalingFactor * glm::vec2(dimX, dimY);
}

glm::vec2 Map::GetWorldCoord(glm::ivec2 mapCoord)
{
	return GetWorldCoord(mapCoord.x, mapCoord.y);
}

glm::vec2 Map::GetWorldCoord(int x, int y)
{
	return (glm::vec2(x, y) + glm::vec2(.5f, .5f)) * scalingFactor;
}

Room::Room(size_t width, size_t height)
{
	this->width = width;
	this->height = height;
	connected = false;

	this->layout.clear();

	this->layout = std::vector<std::vector<int>>(this->width, std::vector<int>(this->height, 1));
}

Room::Room(RoomTemplate roomTemplate)
{
	this->width = roomTemplate.width;
	this->height = roomTemplate.height;
	this->layout = roomTemplate.layout;
}

void Room::SetPosition(size_t x, size_t y)
{
	this->x = x;
	this->y = y;
}

//Best collision check I know: ! of all the cases where it can't possibly collide
bool Room::Collides(Room other)
{
	return !(
		(x + width + 1)				 < other.x ||
		(y + height + 1)			 < other.y ||
		(other.x + other.width + 1)  < x	   ||
		(other.y + other.height + 1) < y 
		);
}

bool Room::is_inside(glm::vec2 position)
{
	size_t xPos = size_t(position.x / map->scalingFactor);
	size_t yPos = size_t(position.y / map->scalingFactor);
	return !(xPos < x || xPos >= (x + width) || yPos < y || yPos >= (y + width));
}

bool Room::is_inside(size_t xPos, size_t yPos)
{
	return !(xPos < x || xPos >= (x + width) || yPos < y || yPos >= (y + width));
}

void Room::Write()
{
	for (size_t xIndex = 0; xIndex < width; xIndex++)
	{
		for (size_t yIndex =  0; yIndex < height; yIndex++)
		{
			map->SetAt(xIndex + x, yIndex + y, layout[xIndex][yIndex]);
		}
	}
}

void Room::WriteDoors()
{
	for (size_t xIndex = x - 1; xIndex <= x + width; xIndex++)
	{
		if (map->ValueAt(xIndex, y - 1) == 1)
		{
			map->SetAt(xIndex, y - 1, 2);
		}
		if (map->ValueAt(xIndex, y + height) == 1)
		{
			map->SetAt(xIndex, y + height, 2);
		}
	}

	for (size_t yIndex = y - 1; yIndex <= y + height; yIndex++)
	{
		if (map->ValueAt(x - 1, yIndex) == 1)
		{
			map->SetAt(x - 1, yIndex, 2);
		}
		if (map->ValueAt(x + width, yIndex))
		{
			map->SetAt(x + width, yIndex, 2);
		}
	}
}

glm::ivec2 Room::GetCenter()
{
	return glm::ivec2(x + (width / 2), y + (height / 2));
}

void Room::SetMap(Map* map)
{
	this->map = map;
}

std::vector<glm::vec2> Room::CreatePositions(size_t numPositions)
{
	std::vector<glm::vec2> positions;
	for (size_t i = 0; i < numPositions; i++)
	{
		while (true)
		{
			size_t xVal = rand() % width;
			size_t yVal = rand() % height;
			if (!map->collision[x + xVal][y + yVal])
			{
				positions.push_back((glm::vec2(x + xVal, y + yVal) + glm::vec2(0.5, 0.5)));
				break;
			}
		}
	}

	return positions;
}

void Room::LockRoom()
{
	locked = true;
	for (size_t xIndex = x - 1; xIndex < x + width + 1; xIndex++)
	{
		for (size_t yIndex = y - 1; yIndex < y + height + 1; yIndex++)
		{
			if (map->ValueAt(xIndex, yIndex) == doorNum)
			{
				map->SetAt(xIndex, yIndex, closedNum);
			}
		}
	}
}

void Room::UnlockRoom()
{
	locked = false;
	for (size_t xIndex = x - 1; xIndex < x + width + 1; xIndex++)
	{
		for (size_t yIndex = y - 1; yIndex < y + height + 1; yIndex++)
		{
			if (map->ValueAt(xIndex, yIndex) == closedNum)
			{
				map->SetAt(xIndex, yIndex, doorNum);
			}
		}
	}
}



//------------------Room template code-------------------------
RoomTemplate::RoomTemplate(std::string path)
{

	std::ifstream input(data_path("prefabs/" + path + ".txt"));

	input >> this->width >> this->height;
	layout = std::vector<std::vector<int>>(width, std::vector<int>(height, -1));

	char val;
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			input >> val;
			//printf("Val is %c\n", val);
			layout[x][y] = atoi(&val);
		}
	}

	input.close();

}