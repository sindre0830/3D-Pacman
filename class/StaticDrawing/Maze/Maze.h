#ifndef __MAZE_H
#define __MAZE_H
/* library */
#include "../StaticDrawing.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
/**
 * @brief Leaf class
 * 
 */
class Maze : public StaticDrawing {
    private:
        /* private data */
        GLuint cornerVAO, shaderProgram3D, vao3D;
        int wallSize = 0, cornerSize = 0, wallSize3D = 0;
        /* private functionality */
        std::vector<GLfloat> genWallCoordinates();
        GLuint genCornerVAO();
        std::vector<GLfloat> genWallCoordinates3D();
    public:
        /* public functionality */
        ~Maze();
        Maze(glm::mat4 collectionMatrix);
        void draw(glm::mat4 collectionMatrix);
};
#endif
