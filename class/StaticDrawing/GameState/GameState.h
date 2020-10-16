#ifndef __GAMESTATE_H
#define __GAMESTATE_H
/* library */
#include "../StaticDrawing.h"
/**
 * @brief Leaf class
 * 
 */
class GameState : public StaticDrawing {
    private:
        /* private functionality */
        std::vector<GLfloat> genCoordinates();
    public:
        /* public functionality */
        ~GameState();
        GameState();
        void draw();
};
#endif
