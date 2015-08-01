#ifndef DUNGEONS_DUNGEON_H_INCLUDED
#define DUNGEONS_DUNGEON_H_INCLUDED


struct areas;
struct rnd;
struct tiles;


struct Dungeon {
    struct areas *areas;
    struct tiles *tiles;
};


/// Caller is responsible for freeing the array but not its contents.
/// The array is NULL terminated.
char const **dungeonAreaDescriptions(struct Dungeon *dungeon);

void finalizeDungeon(struct Dungeon *dungeon);

void generateDungeon(struct Dungeon *dungeon, struct rnd *rnd);

void generateSmallDungeon(struct Dungeon *dungeon);

void initializeDungeon(struct Dungeon *dungeon);


#endif /* #ifndef DUNGEONS_DUNGEON_H_INCLUDED */
