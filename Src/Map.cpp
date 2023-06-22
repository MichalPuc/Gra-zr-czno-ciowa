#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS/ECS.h"
#include "ECS/Components.h"

// Access the manager instance from external source
extern Manager manager;

// Constructor: Initializes the Map with the map scale and tile size
Map::Map(int ms, int ts) : mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
}

// Destructor
Map::~Map()
{

}

// Load the map from a file
void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char tile;
	std::fstream mapFile;
	mapFile.open(path);

	// Iterate over each tile in the map file
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(tile);
			AddTile(atoi(&tile), x * scaledSize, y * scaledSize);
			mapFile.ignore();
		}
	}
	mapFile.ignore();

	// Iterate over each tile in the map file for colliders
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(tile);
			if (atoi(&tile) == '3')
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
		}
	}
	mapFile.close();
}

// Add a tile to the map
void Map::AddTile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, id, 64, 1);
	tile.addGroup(Game::groupMap);
}
