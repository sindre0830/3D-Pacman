#ifndef __PELLET_H
#define __PELLET_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
/**
 * @brief Leaf class
 * 
 */
class Pellet {
    private:
        /* private data */
        GLuint shaderProgram = 0;
        GLuint VAO = 0;
        std::map<std::pair<int, int>, int> bufferPos;
        const int pelletByteSize = ((6 * 4) * 5) * sizeof(GLfloat);
        /* private functionality */
        void drawMirror(const float xMirror, const float yMirror);
        std::vector<GLfloat> genCoordinates();
    public:
        /* public functionality */
        ~Pellet();
        Pellet();
        void draw();
        void hidePellet(const int col, const int row);
};
#endif
