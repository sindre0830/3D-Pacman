#ifndef __DICTIONARY_H_
#define __DICTIONARY_H_
/* dictionary */
enum Direction {UP, LEFT, DOWN, RIGHT};
enum Corner {TOP_LEFT, BOTTOM_LEFT, BOTTOM_RIGHT, TOP_RIGHT};
enum Position {X, Y};
enum Target {PELLET, WALL, PACMAN, EMPTY, MAGICPELLET, VOID};
enum Gamemode {TWO_DIMENSIONAL, FIRST_PERSON, THIRD_PERSON};
enum Sprite {CHARACTER_TEXTURE, NUMBER_TEXTURE, GAMEOVER_TEXTURE, WALL_TEXTURE, LOADING_TEXTURE, MENU_TEXTURE};
#endif
