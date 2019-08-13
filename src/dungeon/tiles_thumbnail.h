#ifndef FNF_DUNGEON_TILES_THUMBNAIL_H_INCLUDED
#define FNF_DUNGEON_TILES_THUMBNAIL_H_INCLUDED


struct tile;


char *
tiles_thumbnail_types_alloc(struct tile **tiles, int tiles_count);

char *
tiles_thumbnail_walls_alloc(struct tile **tiles, int tiles_count);


#endif
