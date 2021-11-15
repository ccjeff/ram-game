#include "DungeonGenerator.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

bool collisionMat[] = { true, false, false };

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
	printf("Starting dungeon gen!\n");
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

	printf("Made all the rooms!\n");

	for (Room r : rooms)
	{
		r.SetMap(&map);
		r.Write();
		map.rooms.push_back(r);
	}

	printf("Wrote all the rooms!\n");

	ConnectRooms();

	int playerRoom = rand() % rooms.size();
	playerStart = rooms[playerRoom].GetCenter();
	rooms.erase(std::next(rooms.begin(), playerRoom));

	for (Room r : rooms)
	{
		monsterPositions.push_back(r.GetCenter());
	}

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

void DungeonGenerator::PrintMap()
{
	for (size_t y = 0; y < dimY; y++)
	{
		for (size_t x = 0; x < dimX; x++)
		{
			printf("%d", map.ValueAt(x, y));
		}
		printf(" - %zu\n", y);
	}

	for (size_t y = 0; y < dimY; y++)
	{
		for (size_t x = 0; x < dimX; x++)
		{
			if (map.collision[x][y])
			{
				printf("X");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
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
	spriteMap = std::vector<std::vector<int>>(dimX, std::vector<int>(dimY, 0));
}

void Map::SetAt(size_t x, size_t y, int value)
{
	map[x][y] = value;
	SetCollisionAt(x, y, collisionMat[value]);
}

void Map::SetCollisionAt(size_t x, size_t y, bool value)
{
	collision[x][y] = value;
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

	for (int i = 0; i < layout.size(); i++)
	{
		for (int j = 0; j < layout[0].size(); j++)
		{
			printf("%d", layout[i][j]);
		}
		printf("\n");
	}
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
		(x + width)				 < other.x ||
		(y + height)			 < other.y ||
		(other.x + other.width)  < x	   ||
		(other.y + other.height) < y 
		);
}

bool Room::IsIn(glm::vec2 position)
{
	size_t xPos = size_t(position.x / map->scalingFactor);
	size_t yPos = size_t(position.y / map->scalingFactor);
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

glm::ivec2 Room::GetCenter()
{
	return glm::ivec2(x + (width / 2), y + (height / 2));
}

void Room::SetMap(Map* map)
{
	this->map = map;
}

void Room::LockRoom()
{
	for (size_t xIndex = x; xIndex < x + width; xIndex++)
	{
		for (size_t yIndex = y; yIndex < y + height; yIndex++)
		{
			if (map->ValueAt(xIndex, yIndex) == doorNum)
			{
				map->SetCollisionAt(xIndex, yIndex, true);
			}
		}
	}
}

void Room::UnlockRoom()
{
	for (size_t xIndex = x; xIndex < x + width; xIndex++)
	{
		for (size_t yIndex = y; yIndex < y + height; yIndex++)
		{
			if (map->ValueAt(xIndex, yIndex) == doorNum)
			{
				map->SetCollisionAt(xIndex, yIndex, false);
			}
		}
	}
}



//------------------Room template code-------------------------
RoomTemplate::RoomTemplate(std::string path)
{
	std::string filepath = data_path("prefabs/" + path + ".txt");
	FILE* input;
	
	if (fopen_s(&input, &filepath.at(0), "r") == 0)
	{

		fscanf_s(input, "%zu %zu\n", &this->width, &this->height);
		layout = std::vector<std::vector<int>>(width, std::vector<int>(height, -1));
		//char* line = (char*)malloc(width * 5 * sizeof(char));
		/*
		for (size_t y = 0; y < height; y++)
		{
			fscanf_s(input, "%s\n", line, uint32_t(width * sizeof(char)));
			for (size_t x = 0; x < width; x++)
			{
				char val = line[x];
				
				layout[x][y] = atoi(&val);
				printf("%zu, %zu - %d\n", x, y, atoi(&val));
			}
		}*/
		char val;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				fscanf_s(input, "%c", &val, 1);
				while (val == '\n')
				{
					fscanf_s(input, "%c", &val, 1);
				}
				layout[x][y] = atoi(&val);
			}
		}

		fclose(input);
	}
	else
	{
		printf("Something went horribly wrong! File could not be opened\n");
	}

	printf("Read a room! Here it is:\n");
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			printf("%d", layout[j][i]);
		}
		printf("\n");
	}
}