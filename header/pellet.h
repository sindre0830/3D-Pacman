#ifndef __PELLET_H
#define __PELLET_H
/* library */
#include "header/geometricShape.h"
#include <map>
/**
 * @brief Leaf class
 * 
 */
class Pellet : public GeometricShape {
    private:
        /* private data */
        std::map<std::pair<int, int>, int> bufferPos;
        const int pelletByteSize = 3 * 4 * sizeof(GLfloat);
        /* private functionality */
        std::vector<GLfloat> genCoordinates();
    public:
        /* public functionality */
        ~Pellet();
        Pellet();
        void draw();
        void hidePellet(const int col, const int row);
};
#endif
