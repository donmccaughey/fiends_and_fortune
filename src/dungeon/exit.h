#ifndef FNF_DUNGEON_EXIT_H_INCLUDED
#define FNF_DUNGEON_EXIT_H_INCLUDED


#include "box.h"
#include "direction.h"
#include "point.h"
#include "wall_type.h"


struct generator;


struct exit {
    enum direction direction;
    struct point point;
    enum wall_type type;
};


int
exits(struct generator *generator,
      struct box box,
      struct exit *exits,
      int exits_count);

int
exits_in_direction(struct generator *generator,
                   struct box box,
                   enum direction direction,
                   struct exit *exits,
                   int exits_count);

int
exits_north(struct generator *generator,
            struct box box,
            struct exit *exits,
            int exits_count);

int
exits_south(struct generator *generator,
            struct box box,
            struct exit *exits,
            int exits_count);

int
exits_east(struct generator *generator,
           struct box box,
           struct exit *exits,
           int exits_count);

int
exits_west(struct generator *generator,
           struct box box,
           struct exit *exits,
           int exits_count);

int
possible_exits(struct generator *generator,
               struct box box,
               struct exit *exits,
               int exits_count);

int
possible_exits_in_direction(struct generator *generator,
                            struct box box,
                            enum direction direction,
                            struct exit *exits,
                            int exits_count);

int
possible_exits_north(struct generator *generator,
                     struct box box,
                     struct exit *exits,
                     int exits_count);

int
possible_exits_south(struct generator *generator,
                     struct box box,
                     struct exit *exits,
                     int exits_count);

int
possible_exits_east(struct generator *generator,
                    struct box box,
                    struct exit *exits,
                    int exits_count);

int
possible_exits_west(struct generator *generator,
                    struct box box,
                    struct exit *exits,
                    int exits_count);


#endif
