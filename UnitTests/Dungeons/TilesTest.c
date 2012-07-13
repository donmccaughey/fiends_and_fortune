#include <assert.h>
#include "Tile.h"
#include "Tiles.h"


void tilesTest(void);

static void addTileToTilesTest(void);
static void tilesOnLevel_when_empty_Test(void);
static void tilesOnLevel_one_tile_Test(void);
static void tilesOnLevel_one_tile_not_found_Test(void);
static void tilesOnLevel_with_two_levels_Test(void);
static void tilesOnLevel_with_two_noncontiguous_levels_Test(void);


void tilesTest(void)
{
  addTileToTilesTest();
  tilesOnLevel_when_empty_Test();
  tilesOnLevel_one_tile_Test();
  tilesOnLevel_one_tile_not_found_Test();
  tilesOnLevel_with_two_levels_Test();
  tilesOnLevel_with_two_noncontiguous_levels_Test();
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


static void tilesOnLevel_when_empty_Test(void)
{
  struct Tiles tiles;

  initializeTiles(&tiles);

  struct Tiles const level1 = tilesOnLevel(&tiles, 1);

  assert(NULL == level1.tiles);
  assert(0 == level1.capacity);
  assert(0 == level1.count);

  finalizeTiles(&tiles);
}


static void tilesOnLevel_one_tile_Test(void)
{
  struct Tiles tiles;

  initializeTiles(&tiles);
  struct Tile tile1 = { .point = { 0, 0, 1 } };
  addTileToTiles(&tiles, &tile1);

  struct Tiles const level1 = tilesOnLevel(&tiles, 1);

  assert(tiles.tiles == level1.tiles);
  assert(0 == level1.capacity);
  assert(1 == level1.count);

  finalizeTiles(&tiles);
}


static void tilesOnLevel_one_tile_not_found_Test(void)
{
  struct Tiles tiles;

  initializeTiles(&tiles);
  struct Tile tile1 = { .point = { 0, 0, 1 } };
  addTileToTiles(&tiles, &tile1);

  struct Tiles const level1 = tilesOnLevel(&tiles, 2);

  assert(NULL == level1.tiles);
  assert(0 == level1.capacity);
  assert(0 == level1.count);

  finalizeTiles(&tiles);
}


static void tilesOnLevel_with_two_levels_Test(void)
{
  struct Tiles tiles;

  initializeTiles(&tiles);
  struct Tile tile = { .point = { 0, 0, 1 } };
  addTileToTiles(&tiles, &tile);
  tile.point = (struct Point) { 0, 1, 1 };
  addTileToTiles(&tiles, &tile);
  tile.point = (struct Point) { 0, 0, 2 };
  addTileToTiles(&tiles, &tile);
  tile.point = (struct Point) { 1, 0, 2 };
  addTileToTiles(&tiles, &tile);
  tile.point = (struct Point) { 2, 0, 2 };
  addTileToTiles(&tiles, &tile);


  struct Tiles const level0 = tilesOnLevel(&tiles, 0);
  assert(NULL == level0.tiles);
  assert(0 == level0.capacity);
  assert(0 == level0.count);

  struct Tiles const level1 = tilesOnLevel(&tiles, 1);
  assert(tiles.tiles == level1.tiles);
  assert(0 == level1.capacity);
  assert(2 == level1.count);
  for (size_t i = 0; i < level1.count; ++i) {
    struct Tile *level1Tile = &level1.tiles[i];
    assert(1 == level1Tile->point.z);
  }

  struct Tiles const level2 = tilesOnLevel(&tiles, 2);
  assert(tiles.tiles != level2.tiles);
  assert(0 == level2.capacity);
  assert(3 == level2.count);
  for (size_t i = 0; i < level2.count; ++i) {
    struct Tile *level2Tile = &level2.tiles[i];
    assert(2 == level2Tile->point.z);
  }

  struct Tiles const level3 = tilesOnLevel(&tiles, 3);
  assert(NULL == level3.tiles);
  assert(0 == level3.capacity);
  assert(0 == level3.count);

  finalizeTiles(&tiles);
}


static void tilesOnLevel_with_two_noncontiguous_levels_Test(void)
{
  struct Tiles tiles;

  initializeTiles(&tiles);
  struct Tile tile = { .point = { 0, 0, 1 } };
  addTileToTiles(&tiles, &tile);
  tile.point = (struct Point) { 0, 1, 1 };
  addTileToTiles(&tiles, &tile);
  tile.point = (struct Point) { 0, 0, 3 };
  addTileToTiles(&tiles, &tile);
  tile.point = (struct Point) { 1, 0, 3 };
  addTileToTiles(&tiles, &tile);
  tile.point = (struct Point) { 2, 0, 3 };
  addTileToTiles(&tiles, &tile);


  struct Tiles const level0 = tilesOnLevel(&tiles, 0);
  assert(NULL == level0.tiles);
  assert(0 == level0.capacity);
  assert(0 == level0.count);

  struct Tiles const level1 = tilesOnLevel(&tiles, 1);
  assert(tiles.tiles == level1.tiles);
  assert(0 == level1.capacity);
  assert(2 == level1.count);
  for (size_t i = 0; i < level1.count; ++i) {
    struct Tile *level1Tile = &level1.tiles[i];
    assert(1 == level1Tile->point.z);
  }

  struct Tiles const level2 = tilesOnLevel(&tiles, 2);
  assert(NULL == level2.tiles);
  assert(0 == level2.capacity);
  assert(0 == level2.count);

  struct Tiles const level3 = tilesOnLevel(&tiles, 3);
  assert(tiles.tiles != level3.tiles);
  assert(0 == level3.capacity);
  assert(3 == level3.count);
  for (size_t i = 0; i < level3.count; ++i) {
    struct Tile *level2Tile = &level3.tiles[i];
    assert(3 == level2Tile->point.z);
  }

  struct Tiles const level4 = tilesOnLevel(&tiles, 4);
  assert(NULL == level4.tiles);
  assert(0 == level4.capacity);
  assert(0 == level4.count);

  finalizeTiles(&tiles);
}
