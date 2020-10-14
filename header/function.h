#ifndef __FUNCTION_H
#define __FUNCTION_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
/* functionality */
void getGhostPos(std::vector<std::vector<int>> &arr, int &row, int &col);
int randomIndex(const int min, const int max);
GLuint loadTexture(const std::string &filepath, const GLuint slot);
void enableDebug();
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif
