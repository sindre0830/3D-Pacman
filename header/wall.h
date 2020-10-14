#ifndef __WALL_H
#define __WALL_H
/* library */
#include "header/geometricShape.h"
/**
 * @brief Leaf class
 * 
 */
class Wall : public GeometricShape {
    private:
        /* private data */
        GLuint cornerVAO;
        int wallSize = 0, cornerSize = 0;
        /* private functionality */
        std::vector<GLfloat> genWallCoordinates();
        GLuint genCornerVAO();
    public:
        /* public functionality */
        ~Wall();
        Wall();
        void draw();
};
#endif
