#ifndef __FUNCTION_H
#define __FUNCTION_H
/* library */
#include "../class/StaticDrawing/GameState/GameState.h"
#include "../class/DynamicDrawing/Score/Score.h"
#include "../class/StaticDrawing/Pellet/Pellet.h"
#include "../class/DynamicDrawing/Character/Pacman/Pacman.h"
#include "../class/DynamicDrawing/Character/Ghost/Ghost.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
/* functionality */
GLuint compileShader(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc);
GLuint genObject(const std::vector<GLfloat> &arr, const int size);
GLuint createVAO(const std::vector<GLfloat> &arr, const std::vector<GLuint> &arr_indices);
std::vector<GLuint> genIndices(const int size);
void destroyVAO(GLuint &vao);

glm::mat4 getMinimapModelMatrix();
void rotateWorld(glm::mat4 &modelMatrix, const int direction);
GLuint loadModel(const std::string path, const std::string file, int &size);
void changeDimension(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void getGhostPos(std::vector<std::vector<int>> &arr, int &row, int &col);
int randomIndex(const int min, const int max);
GLuint loadTexture(const std::string &filepath, const GLuint slot);
void enableDebug();
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif
