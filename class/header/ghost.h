#ifndef __GHOST_H
#define __GHOST_H
/* library */
#include "character.h"
/**
 * @brief Leaf class
 * 
 */
class Ghost : public Character {
    private:
        /* private data */
        int rowPos = 0, colPos = 0;
        /* private functionality */
        void pathfinding();
        void getRandomPath();
        void animate();
        void checkCoalition(const int row, const int col);
    public:
        /* public data */
        bool dead = false;
        /* public functionality */
        ~Ghost();
        Ghost(const int row, const int col);
        void mov();
        void changeColor(const int flag);
};
#endif
