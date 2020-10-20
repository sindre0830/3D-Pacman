#ifndef __PACMAN_H
#define __PACMAN_H
/* library */
#include "../Character.h"
#include "../../../StaticDrawing/Pellet/Pellet.h"
/**
 * Leaf class
 */
class Pacman : public Character {
    private:
        /* private data */
        bool changeDirection;
        /* private functionality */
        void getPosition();
        void animate();
        void eat(Pellet &pellet);
    public:
        /* public functionality */
        ~Pacman();
        Pacman();
        void mov(Pellet &pellet);
        void inputDirection(GLFWwindow *window);
};
#endif
