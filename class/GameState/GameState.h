#ifndef __GAMESTATE_H
#define __GAMESTATE_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
/**
 * @brief Leaf class
 * 
 */
class GameState {
    private:
        /* private data */
        GLuint shaderProgram;
        GLuint VAO;
        /* private functionality */
        std::vector<GLfloat> genCoordinates();
    public:
        /* public functionality */
        ~GameState();
        GameState();
        void draw(const int texIndex);
};
#endif
