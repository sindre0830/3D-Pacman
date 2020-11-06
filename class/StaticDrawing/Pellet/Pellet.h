#ifndef __PELLET_H
#define __PELLET_H
/* library */
#include "../StaticDrawing.h"
#include <map>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
/**
 * @brief Leaf class
 * 
 */
class Pellet : public StaticDrawing {
    private:
        /* private data */
        std::map<std::pair<int, int>, int> bufferPos;
        const int pelletByteSize = 3 * 4 * sizeof(GLfloat);
        int modelSize;
        GLuint modelVAO;
        /* private functionality */
        std::vector<GLfloat> genCoordinates();
    public:
        /* public functionality */
        ~Pellet();
        Pellet(glm::mat4 modelMatrix, glm::mat4 projectionMatrix);
        void draw(glm::mat4 modelMatrix, glm::mat4 projectionMatrix);
        void hidePellet(const int col, const int row);
};
#endif
