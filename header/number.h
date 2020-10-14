#ifndef __NUMBER_H
#define __NUMBER_H
/* library */
#include "header/geometricShape.h"
/**
 * @brief Leaf class
 * 
 */
class Number : public GeometricShape {
    private:
        /* private data */
        int lastNumber = 0;
        /* private functionality */
        std::vector<GLfloat> genCoordinates(const int col, const int row);
        void translateTex(const float xPos);
    public:
        /* public functionality */
        ~Number();
        Number(const int col, const int row);
        void draw();
        void update(const int num);
};
#endif