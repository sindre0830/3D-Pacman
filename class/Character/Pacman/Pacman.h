#ifndef __PACMAN_H
#define __PACMAN_H
/* library */
#include "../Character.h"
#include "../../Pellet/Pellet.h"
/**
 * Leaf class
 */
class Pacman : public Character {
    private:
        /* private data */
        bool changeDirection = false;
        /* private functionality */
        void getPosition();
        void animate();
        void eat(Pellet &pellet);
        void updateCameraPosition();
    public:
        /* public functionality */
        ~Pacman();
        Pacman();
        void mov(Pellet &pellet);
        void inputDirection(GLFWwindow *window);
};
#endif
