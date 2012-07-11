#include <assert.h>
#include "Tile.h"
#include "Tiles.h"


void tilesTest(void);

static void addTileToTilesTest(void);


void tilesTest(void)
{
  addTileToTilesTest();
}


static void addTileToTilesTest(void)
{
  struct Tiles tiles;
  
  initializeTiles(&tiles);
  
  assert(0 == tiles.capacity);
  assert(0 == tiles.count);
  assert(tiles.tiles);
  
  struct Tile tile1 = { .x = 0 };
  addTileToTiles(&tiles, &tile1);
  
  assert(tiles.capacity > 0);
  assert(1 == tiles.count);
  assert(tiles.tiles[0] == &tile1);
  
  struct Tile tile2 = { .x = 1 };
  addTileToTiles(&tiles, &tile2);
  
  assert(2 == tiles.count);
  assert(tiles.tiles[1] == &tile2);
  
  struct Tile tile3 = { .x = -1 };
  addTileToTiles(&tiles, &tile3);
  
  assert(3 == tiles.count);
  assert(tiles.tiles[0] == &tile3);
  assert(tiles.tiles[1] == &tile1);
  assert(tiles.tiles[2] == &tile2);
  
  Boolean found = removeTileFromTiles(&tiles, &tile1);
  assert(found);
  assert(2 == tiles.count);
  assert(tiles.tiles[0] == &tile3);
  assert(tiles.tiles[1] == &tile2);
  
  found = removeTileFromTiles(&tiles, &tile2);
  assert(found);
  assert(1 == tiles.count);
  assert(tiles.tiles[0] == &tile3);
  
  found = removeTileFromTiles(&tiles, &tile3);
  assert(found);
  assert(0 == tiles.count);
  
  finalizeTiles(&tiles);
}
