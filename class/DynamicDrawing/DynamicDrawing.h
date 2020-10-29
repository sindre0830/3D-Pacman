#ifndef __DYNAMICDRAWING_H
#define __DYNAMICDRAWING_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
/**
 * @brief Root class
 * 
 */
class DynamicDrawing {
    protected:
        /* protected data */
        GLuint shaderProgram;
        GLuint VAO;
        /* protected functionality */
        GLuint compileShader(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc);
        GLuint genObject(const std::vector<GLfloat> arr, const int size);
        GLuint createVAO(const std::vector<GLfloat> &arr, const std::vector<GLuint> &arr_indices);
        std::vector<GLuint> genIndices(const int size);
        void destroyVAO(GLuint &vao);
    public:
        /* public functionality */
        ~DynamicDrawing();
};
#endif