#ifndef __GEOMETRICSHAPE_H
#define __GEOMETRICSHAPE_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
/**
 * @brief Root class
 * 
 */
class GeometricShape {
    protected:
        /* protected data */
        GLuint shapeShaderProgram;
        GLuint shapeVAO;
        /* protected functionality */
        GLuint compileShader(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc);
        GLuint genObject(const std::vector<GLfloat> arr, const int size);
        GLuint createVAO(const std::vector<GLfloat> &arr, const std::vector<GLuint> &arr_indices);
        std::vector<GLuint> genIndices(const int size);
        void destroyVAO(GLuint &vao);
    public:
        /* public functionality */
        ~GeometricShape();
};
#endif
