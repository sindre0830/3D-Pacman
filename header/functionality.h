#ifndef __FUNCTION_H
#define __FUNCTION_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
/* functionality */
glm::mat4 getMinimapModelMatrix();
void changeDimension(GLFWwindow* window);
void getGhostPos(std::vector<std::vector<int>> &arr, int &row, int &col);
int randomIndex(const int min, const int max);
GLuint genObject(const std::vector<GLfloat> &arr, const int size);
std::vector<GLuint> genIndices(const int size);
GLuint compileShader(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc);
GLuint loadModel(const std::string path, const std::string file, int &size);
GLuint loadTexture(const std::string &filepath, const GLuint slot);
GLuint createVAO(const std::vector<GLfloat> &arr, const std::vector<GLuint> &arr_indices);
void destroyVAO(GLuint &VAO);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void enableDebug();
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif
