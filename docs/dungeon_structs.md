dungeon Structs
---------------

               +---------+
               |         |
               | dungeon |
          +---+|         |+----+
          |    +---------+     |
          |                    |
          |                    |
          v                    v
        +-------+            +-------+
        |       |            |       |
        | areas |            | tiles |
        |       |            |       |
        +-------+            +-------+
          +                  +   +
          |                  |   |
          |   +--------------+   |
          |   |                  |
          v   v                  v
        +-------+            +------+
        |       |            |      |
        | tiles |+---------->| tile |
        |       |            |      |
        +-------+            +------+
