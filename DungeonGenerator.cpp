#include "DungeonGenerator.hpp"

DungeonGenerator::DungeonGenerator(size_t x, size_t y)
{
	dimX = x;
	dimY = y;

	map = (int*) malloc(sizeof(int) * dimX * dimY);
	for (size_t xIndex = 0; xIndex < dimX; xIndex++)
	{
		for (size_t yIndex = 0; yIndex < dimY; yIndex++)
		{
			map[index(xIndex, yIndex)] = 0;
		}
	}
}

DungeonGenerator::~DungeonGenerator()
{
	free(map);
}

void DungeonGenerator::SetAt(size_t x, size_t y, int value)
{
	map[index(x, y)] = value;
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
			
			size_t width  = (rand() % (maxRoomSize - minRoomSize)) + minRoomSize;
			size_t height = (rand() % (maxRoomSize - minRoomSize)) + minRoomSize;
			size_t startX = (rand() % (dimX - width  - 2)) + 1;
			size_t startY = (rand() % (dimY - height - 2)) + 1;

			Room newRoom = Room(startX, startY, width, height);

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
			printf("Generator failed to place room after %zu tries. Aborting dungeon gen.\n", attemptsPerRoom);
			return false;
		}
	}

	printf("Generator succeeded in finding positiosn for all rooms.\n");
	for (Room r : rooms)
	{
		r.Write(this);
	}

	for (int i = 0; i < rooms.size() - 1; i++)
	{
		DrawCorridor(rooms[i], rooms[i + 1]);
	}

	playerStart = rooms[0].GetCenter();

	printf("Generator succeeded in writing all rooms.\n");
	PrintMap();

	return true;
}

void DungeonGenerator::DrawCorridor(Room one, Room two)
{
	glm::ivec2 first = one.GetCenter();
	glm::ivec2 second = two.GetCenter();
	if (rand() % 1 == 0)
	{
		//Horizontal corridor
		int halfway = (first.x + second.x) / 2;

		for (int i = std::min(first.x, halfway); i <= std::max(first.x, halfway); i++)
		{
			map[index(i, first.y)] = 1;
		}

		for (int j = std::min(first.y, second.y); j <= std::max(first.y, second.y); j++)
		{
			map[index(halfway, j)] = 1;
		}

		for (int i = std::min(second.x, halfway); i <= std::max(second.x, halfway); i++)
		{
			map[index(i, second.y)] = 1;
		}
	}
	else
	{
		//Vertical corridor
	}
}

void DungeonGenerator::PrintMap()
{
	for (size_t y = 0; y < dimY; y++)
	{
		for (size_t x = 0; x < dimX; x++)
		{
		
			printf("%d", map[index(x, y)]);
		}
		printf(" - %zu\n", y);
	}
}


Room::Room(size_t x, size_t y, size_t width, size_t height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

//Best collision check I know: ! of all the cases where it can't possibly collide
bool Room::Collides(Room other)
{
	return !(
		(x + width)				 < other.x  ||
		(y + height)			 < other.y ||
		(other.x + other.width)  < x ||
		(other.y + other.height) < y 
		);
}

void Room::Write(DungeonGenerator *gen)
{
	for (size_t xIndex = x; xIndex < (x + width); xIndex++)
	{
		for (size_t yIndex = y; yIndex < (y + height); yIndex++)
		{
			gen->SetAt(xIndex, yIndex, 1);
		}
	}

}

glm::ivec2 Room::GetCenter()
{
	return glm::ivec2(x + (width / 2), y + (width / 2));
}