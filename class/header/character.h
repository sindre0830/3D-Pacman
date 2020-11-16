#ifndef __CHARACTER_H
#define __CHARACTER_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
/**
 * @brief Internal class
 * 
 */
class Character {
    protected:
        /* protected data */
        GLuint shaderProgram2D = 0, shaderProgram3D = 0;
        GLuint spriteVAO = 0, modelVAO = 0;
        int counter = 0, speed = 20, modelSize = 0;
        float xPos = 0.f, yPos = 0.f, yTex = 0.f;
        float camX = 0.f, camY = 0.f, modelDirection = 0.f;
        glm::mat4 modelMatrix;
        glm::vec3 translation, initialTranslation;
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
        void draw();
};
#endif
