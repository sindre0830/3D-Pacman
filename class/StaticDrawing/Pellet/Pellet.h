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
        const int pelletByteSize = ((6 * 4) * 5) * sizeof(GLfloat);
        /* private functionality */
        std::vector<GLfloat> genCoordinates();
    public:
        /* public functionality */
        ~Pellet();
        Pellet();
        void draw(glm::mat4 collectionMatrix);
        void hidePellet(const int col, const int row);
};
#endif
