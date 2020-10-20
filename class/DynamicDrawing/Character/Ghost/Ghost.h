#ifndef __GHOST_H
#define __GHOST_H
/* library */
#include "../Character.h"
/**
 * @brief Leaf class
 * 
 */
class Ghost : public Character {
    private:
        /* private data */
        int rowPos, colPos;
        /* private functionality */
        void pathfinding();
        void findRandomPath();
        void animate();
    public:
        /* public data */
        bool dead = false;
        /* public functionality */
        ~Ghost();
        Ghost(const int row, const int col);
        void mov();
        void checkCoalition(const int row, const int col);
        void changeColor(const int flag);
};
#endif
