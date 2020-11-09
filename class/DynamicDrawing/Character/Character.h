#ifndef __CHARACTER_H
#define __CHARACTER_H
/* library */
#include "../DynamicDrawing.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
/**
 * @brief Internal class
 * 
 */
class Character : public DynamicDrawing {
    protected:
        /* protected data */
        int counter = 0, speed = 20;
        float xPos = 0.f, yPos = 0.f, yTex = 0.f;
        float camX, camY;
        glm::mat4 modelMatrix;
        glm::vec3 translation;
        /* protected functionality */
        std::vector<GLfloat> genCoordinates(const int row, const int col);
        void translatePos(const float xPos, const float yPos);
        void translateTex(const float xPos, const float yPos);
        bool movUp(int &row, int &col);
        bool movLeft(int &row, int &col);
        bool movDown(int &row, int &col);
        bool movRight(int &row, int &col);
    public:
        /* public data */
        int direction;
        /* public functionality */
        ~Character();
        Character();
        void draw(glm::mat4 collectionMatrix);
};
#endif
