#ifndef DUNGEONS_POINT_H_INCLUDED
#define DUNGEONS_POINT_H_INCLUDED


struct Point {
  int x;
  int y;
  int z;
};


int comparePoints(struct Point const *point1, struct Point const *point2);


#endif /* #ifndef DUNGEONS_POINT_H_INCLUDED */
