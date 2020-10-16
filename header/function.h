#ifndef __FUNCTION_H
#define __FUNCTION_H
/* library */
#include "../class/StaticDrawing/GameState/GameState.h"
#include "../class/DynamicDrawing/Score/Score.h"
#include "../class/StaticDrawing/Maze/Maze.h"
#include "../class/StaticDrawing/Pellet/Pellet.h"
#include "../class/DynamicDrawing/Character/Pacman/pacman.h"
#include "../class/DynamicDrawing/Character/Ghost/ghost.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
/* functionality */
void gameloop2D(GLFWwindow *window, Maze *maze, Pellet *pellet, Pacman *pacman, std::vector<Ghost*> ghostArr, const double deltaTime, int &counter);
void gameloop3D(GLFWwindow *window, Maze *maze, Pellet *pellet, Pacman *pacman, std::vector<Ghost*> ghostArr, const double deltaTime, int &counter);
void getGhostPos(std::vector<std::vector<int>> &arr, int &row, int &col);
int randomIndex(const int min, const int max);
GLuint loadTexture(const std::string &filepath, const GLuint slot);
void enableDebug();
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif
