#ifndef __MAZE_H
#define __MAZE_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
/**
 * @brief Leaf class
 * 
 */
class Maze {
    private:
        /* private data */
        GLuint shaderProgram2D = 0, shaderProgram3D = 0;
        GLuint wallVAO = 0, cornerVAO = 0, modelVAO = 0;
        int wallSize = 0, cornerSize = 0, wallSize3D = 0;
        /* private functionality */
        void drawMirror(const float xMirror, const float yMirror);
        std::vector<GLfloat> genWallCoordinates();
        GLuint genCornerVAO();
        std::vector<GLfloat> genWallCoordinates3D();
    public:
        /* public functionality */
        ~Maze();
        Maze();
        void draw();
};
#endif
