#ifndef DUNGEONS_DIRECTION_H_INCLUDED
#define DUNGEONS_DIRECTION_H_INCLUDED


enum Direction {
  Unknown = 0,
  North,
  South,
  East,
  West,
};


char const *directionName(enum Direction direction);


#endif /* #ifndef DUNGEONS_DIRECTION_H_INCLUDED */
