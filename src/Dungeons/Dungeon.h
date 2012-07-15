#ifndef DUNGEONS_DUNGEON_H_INCLUDED
#define DUNGEONS_DUNGEON_H_INCLUDED


struct Dice;
struct Tiles;


struct Dungeon {
  struct Tiles *tiles;
};


void finalizeDungeon(struct Dungeon *dungeon);

void generateDungeon(struct Dungeon *dungeon, struct Dice *dice);

void initializeDungeon(struct Dungeon *dungeon);


#endif /* #ifndef DUNGEONS_DUNGEON_H_INCLUDED */
