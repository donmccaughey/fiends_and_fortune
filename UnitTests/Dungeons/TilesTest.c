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

static bool equalTiles(struct Tile *tile1, struct Tile *tile2);


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
  struct Tiles *tiles = createTiles();
  assert(0 == tilesCount(tiles));
  assert(range_is_equal(range_make(0, 0), xRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 0), yRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 0), zRangeOfTiles(tiles)));

  struct Tile *tile1 = createTile(makePoint(0, 0, 0), EmptyTileType);
  addTileToTiles(tiles, tile1);
  
  assert(1 == tilesCount(tiles));
  assert(tileInTilesAtIndex(tiles, 0)->point.x == tile1->point.x);
  assert(range_is_equal(range_make(0, 1), xRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 1), yRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 1), zRangeOfTiles(tiles)));

  struct Tile *tile2 = createTile(makePoint(1, 0, 0), EmptyTileType);
  addTileToTiles(tiles, tile2);
  
  assert(2 == tilesCount(tiles));
  assert(tileInTilesAtIndex(tiles, 1)->point.x == tile2->point.x);
  assert(range_is_equal(range_make(0, 2), xRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 1), yRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 1), zRangeOfTiles(tiles)));

  struct Tile *tile3 = createTile(makePoint(-1, 0, 0), EmptyTileType);
  addTileToTiles(tiles, tile3);
  
  assert(3 == tilesCount(tiles));
  assert(tileInTilesAtIndex(tiles, 0)->point.x == tile3->point.x);
  assert(tileInTilesAtIndex(tiles, 1)->point.x == tile1->point.x);
  assert(tileInTilesAtIndex(tiles, 2)->point.x == tile2->point.x);
  assert(range_is_equal(range_make(-1, 2), xRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 1), yRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 1), zRangeOfTiles(tiles)));

  bool found = removeTileFromTiles(tiles, tile1);
  assert(found);
  assert(2 == tilesCount(tiles));
  assert(tileInTilesAtIndex(tiles, 0)->point.x == tile3->point.x);
  assert(tileInTilesAtIndex(tiles, 1)->point.x == tile2->point.x);
  assert(range_is_equal(range_make(-1, 2), xRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 1), yRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 1), zRangeOfTiles(tiles)));
  destroyTile(tile1);
  
  found = removeTileFromTiles(tiles, tile2);
  assert(found);
  assert(1 == tilesCount(tiles));
  assert(tileInTilesAtIndex(tiles, 0)->point.x == tile3->point.x);
  assert(range_is_equal(range_make(-1, 0), xRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 1), yRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 1), zRangeOfTiles(tiles)));
  destroyTile(tile2);
  
  found = removeTileFromTiles(tiles, tile3);
  assert(found);
  assert(0 == tilesCount(tiles));
  assert(range_is_equal(range_make(0, 0), xRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 0), yRangeOfTiles(tiles)));
  assert(range_is_equal(range_make(0, 0), zRangeOfTiles(tiles)));
  destroyTile(tile3);
  
  destroyTiles(tiles);
}


static void tilesOnLevel_when_empty_Test(void)
{
  struct Tiles *tiles = createTiles();
  struct Tiles *level1 = createTilesOnLevel(tiles, 1);

  assert(0 == tilesCount(level1));

  destroyTiles(level1);
  destroyTiles(tiles);
}


static void tilesOnLevel_one_tile_Test(void)
{
  struct Tiles *tiles = createTiles();
  struct Tile *tile1 = createTile(makePoint(0, 0, 1), EmptyTileType);
  addTileToTiles(tiles, tile1);
  struct Tiles *level1 = createTilesOnLevel(tiles, 1);

  assert(equalTiles(tileInTilesAtIndex(tiles, 0), tileInTilesAtIndex(level1, 0)));
  assert(1 == tilesCount(level1));

  destroyTiles(level1);
  destroyTiles(tiles);
}


static void tilesOnLevel_one_tile_not_found_Test(void)
{
  struct Tiles *tiles = createTiles();
  struct Tile *tile1 = createTile(makePoint(0, 0, 1), EmptyTileType);
  addTileToTiles(tiles, tile1);
  struct Tiles *level1 = createTilesOnLevel(tiles, 2);

  assert(0 == tilesCount(level1));

  destroyTiles(level1);
  destroyTiles(tiles);
}


static void tilesOnLevel_with_two_levels_Test(void)
{
  struct Tiles *tiles = createTiles();
  struct Tile *tile = createTile(makePoint(0, 0, 1), EmptyTileType);
  addTileToTiles(tiles, tile);
  tile = createTile(makePoint(0, 1, 1), EmptyTileType);
  addTileToTiles(tiles, tile);
  tile = createTile(makePoint(0, 0, 2), EmptyTileType);
  addTileToTiles(tiles, tile);
  tile = createTile(makePoint(1, 0, 2), EmptyTileType);
  addTileToTiles(tiles, tile);
  tile = createTile(makePoint(2, 0, 2), EmptyTileType);
  addTileToTiles(tiles, tile);


  struct Tiles *level0 = createTilesOnLevel(tiles, 0);
  assert(0 == tilesCount(level0));
  destroyTiles(level0);

  struct Tiles *level1 = createTilesOnLevel(tiles, 1);
  assert(equalTiles(tileInTilesAtIndex(tiles, 0), tileInTilesAtIndex(level1, 0)));
  assert(2 == tilesCount(level1));
  for (size_t i = 0; i < tilesCount(level1); ++i) {
    struct Tile *level1Tile = tileInTilesAtIndex(level1, i);
    assert(1 == level1Tile->point.z);
  }
  destroyTiles(level1);

  struct Tiles *level2 = createTilesOnLevel(tiles, 2);
  assert( ! equalTiles(tileInTilesAtIndex(tiles, 0), tileInTilesAtIndex(level2, 0)));
  assert(3 == tilesCount(level2));
  for (size_t i = 0; i < tilesCount(level2); ++i) {
    struct Tile *level2Tile = tileInTilesAtIndex(level2, i);
    assert(2 == level2Tile->point.z);
  }
  destroyTiles(level2);

  struct Tiles *level3 = createTilesOnLevel(tiles, 3);
  assert(0 == tilesCount(level3));
  destroyTiles(level3);

  destroyTiles(tiles);
}


static void tilesOnLevel_with_two_noncontiguous_levels_Test(void)
{
  struct Tiles *tiles = createTiles();
  struct Tile *tile = createTile(makePoint(0, 0, 1), EmptyTileType);
  addTileToTiles(tiles, tile);
  tile = createTile(makePoint(0, 1, 1), EmptyTileType);
  addTileToTiles(tiles, tile);
  tile = createTile(makePoint(0, 0, 3), EmptyTileType);
  addTileToTiles(tiles, tile);
  tile = createTile(makePoint(1, 0, 3), EmptyTileType);
  addTileToTiles(tiles, tile);
  tile = createTile(makePoint(2, 0, 3), EmptyTileType);
  addTileToTiles(tiles, tile);


  struct Tiles *level0 = createTilesOnLevel(tiles, 0);
  assert(0 == tilesCount(level0));
  destroyTiles(level0);

  struct Tiles *level1 = createTilesOnLevel(tiles, 1);
  assert(equalTiles(tileInTilesAtIndex(tiles, 0), tileInTilesAtIndex(level1, 0)));
  assert(2 == tilesCount(level1));
  for (size_t i = 0; i < tilesCount(level1); ++i) {
    struct Tile *level1Tile = tileInTilesAtIndex(level1, i);
    assert(1 == level1Tile->point.z);
  }
  destroyTiles(level1);

  struct Tiles *level2 = createTilesOnLevel(tiles, 2);
  assert(0 == tilesCount(level2));
  destroyTiles(level2);

  struct Tiles *level3 = createTilesOnLevel(tiles, 3);
  assert( ! equalTiles(tileInTilesAtIndex(tiles, 0), tileInTilesAtIndex(level3, 0)));
  assert(3 == tilesCount(level3));
  for (size_t i = 0; i < tilesCount(level3); ++i) {
    struct Tile *level2Tile = tileInTilesAtIndex(level3, i);
    assert(3 == level2Tile->point.z);
  }
  destroyTiles(level3);

  struct Tiles *level4 = createTilesOnLevel(tiles, 4);
  assert(0 == tilesCount(level4));
  destroyTiles(level4);

  destroyTiles(tiles);
}


static bool equalTiles(struct Tile *tile1, struct Tile *tile2)
{
    return tile1->point.x == tile2->point.x
        && tile1->point.y == tile2->point.y
        && tile1->point.z == tile2->point.z
        && tile1->type == tile2->type;
}
