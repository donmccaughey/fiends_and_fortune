#ifndef DUNGEONS_TILES_H_INCLUDED
#define DUNGEONS_TILES_H_INCLUDED


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "Point.h"
#include "range.h"


struct Tile;
struct Tiles;


/// The Tiles struct takes ownership of the given tile.
/// The tile is added to the parent Tiles struct as well.
void addTileToTiles(struct Tiles *tiles, struct Tile *tile);

struct Tiles *createEmptyTilesWithParent(struct Tiles *tiles);

struct Tiles *createTiles(void);

/// The created Tiles struct is a child of the given Tiles struct.
struct Tiles *createTilesOnLevel(struct Tiles *tiles, int32_t z);

void destroyTiles(struct Tiles *tiles);

struct Tile *findTileInTilesAt(struct Tiles const *tiles, struct Point point);

/// The caller is responsible for destroying the tile.
bool removeTileFromTiles(struct Tiles *tiles, struct Tile const *tile);

struct Tile *tileInTilesAtIndex(struct Tiles const *tiles, size_t index);

size_t tilesCount(struct Tiles const *tiles);

struct range xRangeOfTiles(struct Tiles const *tiles);

struct range yRangeOfTiles(struct Tiles const *tiles);

struct range zRangeOfTiles(struct Tiles const *tiles);


#endif /* #ifndef DUNGEONS_TILES_H_INCLUDED */
