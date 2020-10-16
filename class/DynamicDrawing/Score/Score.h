#ifndef __SCORE_H
#define __SCORE_H
/* library */
#include "../DynamicDrawing.h"
/**
 * @brief Leaf class
 * 
 */
class Score : public DynamicDrawing {
    private:
        /* private data */
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