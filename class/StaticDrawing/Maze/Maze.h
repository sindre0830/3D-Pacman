#ifndef __MAZE_H
#define __MAZE_H
/* library */
#include "../StaticDrawing.h"
/**
 * @brief Leaf class
 * 
 */
class Maze : public StaticDrawing {
    private:
        /* private data */
        GLuint cornerVAO;
        int wallSize = 0, cornerSize = 0;
        /* private functionality */
        std::vector<GLfloat> genWallCoordinates();
        GLuint genCornerVAO();
    public:
        /* public functionality */
        ~Maze();
        Maze();
        void draw();
};
#endif
