#ifndef __SCORE_H
#define __SCORE_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
/**
 * @brief Leaf class
 * 
 */
class Score {
    private:
        /* private data */
        GLuint shaderProgram;
        GLuint VAO;
        int lastNumber = 0;
        /* private functionality */
        std::vector<GLfloat> genCoordinates(const int col, const int row);
        void translateTex(const float xPos);
    public:
        /* public functionality */
        ~Score();
        Score(const int col, const int row);
        void draw();
        void update(const int num);
};
#endif