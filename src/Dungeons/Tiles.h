#ifndef DUNGEONS_TILES_H_INCLUDED
#define DUNGEONS_TILES_H_INCLUDED


#include <stddef.h>
#include <stdint.h>
#include "Boolean.h"
#include "Range.h"


struct Tile;
struct Tiles;


/// The Tiles struct takes ownership of the given tile.
void addTileToTiles(struct Tiles *tiles, struct Tile *tile);

struct Tiles *createTiles(void);

struct Tiles *createTilesOnLevel(struct Tiles *tiles, int32_t z);

void destroyTiles(struct Tiles *tiles);

struct Tile *findTileInTilesAt(struct Tiles const *tiles, int32_t x, int32_t y, int32_t z);

/// The caller is responsible for destroying the tile.
Boolean removeTileFromTiles(struct Tiles *tiles, struct Tile const *tile);

struct Tile *tileInTilesAtIndex(struct Tiles const *tiles, size_t index);

size_t tilesCount(struct Tiles const *tiles);

struct Range xRangeOfTiles(struct Tiles const *tiles);

struct Range yRangeOfTiles(struct Tiles const *tiles);

struct Range zRangeOfTiles(struct Tiles const *tiles);


#endif /* #ifndef DUNGEONS_TILES_H_INCLUDED */
