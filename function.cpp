/* library */
#include "header/function.h"
#include "header/levelData.h"
#include "header/Camera.h"
#include <stb_image.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
/* global data */
extern LevelData *g_level;
extern Camera *g_camera;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	g_camera->updateDirection(xpos, ypos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
/**
 * @brief Generate random index and set position
 * 
 * @param arr 
 * @param row 
 * @param col 
 */
void gameloop2D(GLFWwindow *window, Pellet *pellet, Pacman *pacman, std::vector<Ghost*> ghostArr, const double deltaTime, int &counter) {
	//draw pellets
	pellet->draw();
	//draw pacman
	pacman->draw();
	//branch if game isn't over
	if (!g_level->gameover && deltaTime >= 1.0){
		//translate pacman
		pacman->mov(*pellet);
		//check for user input and change direction accordingly
		pacman->inputDirection(window);
	}
	//draw ghosts
	bool noActiveGhosts = true;
	for(int i = 0; i < ghostArr.size(); i++) {
		//branch if ghost isn't dead
		if(!ghostArr[i]->dead) {
			noActiveGhosts = false;
			ghostArr[i]->draw();
			//branch if game isn't over and translate the ghost
			if (!g_level->gameover && deltaTime >= 1.0) ghostArr[i]->mov();
		}
	}
	//branch if there are no more ghosts on the level and end the game
	if(noActiveGhosts) g_level->gameover = true;
}

void gameloop3D(GLFWwindow *window, Pellet *pellet, Pacman *pacman, std::vector<Ghost*> ghostArr, const double deltaTime, int &counter) {
	if(g_level->gamemode == FIRST_PERSON) {
		/* Camera function showing a FPP perspective */
	} else {
		/* Camera function showing a Third-Person perspective */
	}
	/* Draw 3D objects */
}

void getGhostPos(std::vector<std::vector<int>> &arr, int &row, int &col) {
	//get random index
	int index = randomIndex(0, arr.size() - 1);
	//set position
	col = arr[index][0];
	row = arr[index][1];
	//remove position from array so ghosts don't spawn on same tile
	arr.erase(arr.begin() + index);
}

int randomIndex(const int min, const int max) {
	//initialise (seed) engine
	std::random_device rd;
	//random-number engine (Mersenne-Twister)
	std::mt19937 rng(rd());
	//ubiased integer between 0 and array size
	std::uniform_int_distribution<int> uni(min, max);
	return uni(rng);
}
/**
 * @brief Loads texture from filepath.
 * 
 * @param filepath
 * @param slot
 * @return GLuint
 */
GLuint loadTexture(const std::string& filepath, const GLuint slot) {
	//flip image
	stbi_set_flip_vertically_on_load(true);
	//load pixel data from a stored image
    int width, height, bpp;
    auto pixels = stbi_load(filepath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    //generate the texture, activate it and bind it
	GLuint tex{};
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, tex);
    //set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//transfer the image data to the texture in GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	//generate different resolutions for distance
	glGenerateMipmap(GL_TEXTURE_2D);
    //free the memory returned by STBI
    if(pixels) stbi_image_free(pixels);
    return tex;
}
/**
 * @brief Eanable capture of debug output.
 * 
 */
void enableDebug() {
    glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(messageCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
}
/**
 * @brief Customize the error message.
 * 
 * @param source 
 * @param type 
 * @param id 
 * @param severity 
 * @param length 
 * @param message 
 * @param userParam 
 */
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam ) {
	std::cerr 
		<< "GL CALLBACK:" << ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ) 
		<< "type = 0x" << type 
		<< ", severity = 0x" << severity 
		<< ", message =" << message << '\n';
}
