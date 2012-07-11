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
  
  struct Tile tile1 = { .point = { 0, 0, 0 } };
  addTileToTiles(&tiles, &tile1);
  
  assert(tiles.capacity > 0);
  assert(1 == tiles.count);
  assert(tiles.tiles[0].point.x == tile1.point.x);
  
  struct Tile tile2 = { .point = { 1, 0, 0 } };
  addTileToTiles(&tiles, &tile2);
  
  assert(2 == tiles.count);
  assert(tiles.tiles[1].point.x == tile2.point.x);
  
  struct Tile tile3 = { .point = { -1, 0, 0 } };
  addTileToTiles(&tiles, &tile3);
  
  assert(3 == tiles.count);
  assert(tiles.tiles[0].point.x == tile3.point.x);
  assert(tiles.tiles[1].point.x == tile1.point.x);
  assert(tiles.tiles[2].point.x == tile2.point.x);
  
  Boolean found = removeTileFromTiles(&tiles, &tile1);
  assert(found);
  assert(2 == tiles.count);
  assert(tiles.tiles[0].point.x == tile3.point.x);
  assert(tiles.tiles[1].point.x == tile2.point.x);
  
  found = removeTileFromTiles(&tiles, &tile2);
  assert(found);
  assert(1 == tiles.count);
  assert(tiles.tiles[0].point.x == tile3.point.x);
  
  found = removeTileFromTiles(&tiles, &tile3);
  assert(found);
  assert(0 == tiles.count);
  
  finalizeTiles(&tiles);
}
