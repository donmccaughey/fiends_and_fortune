#ifndef FNF_ACTION_H_INCLUDED
#define FNF_ACTION_H_INCLUDED


enum action {
    action_invalid=-1,
    action_none=0,
    action_character,
    action_check,
    action_dungeon,
    action_each,
    action_game,
    action_magic,
    action_map,
    action_table,
    action_treasure,
};


enum action
action_from_string(char const *string);


#endif
