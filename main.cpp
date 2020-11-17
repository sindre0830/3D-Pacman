/**
 * Assignment 2 - Pacman
 * Fully functional Pacman game in 3D space
 *
 * @author Sindre Eiklid (sindreik@stud.ntnu.no)
 * @author Casper Melhus (caspertm@stud.ntnu.no)
 * @author Brage Heimly N�ss
 *
 * @date 2020/11/17
 */
 /* library */
#include "dictionary.h"
#include "level.h"
#include "camera.h"
#include "functionality.h"
#include "class/header/score.h"
#include "class/header/maze.h"
#include "class/header/pellet.h"
#include "class/header/gameState.h"
#include "class/header/pacman.h"
#include "class/header/ghost.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
/* global data */
Level* g_level;
Camera* g_camera;
/**
 * Main program.
 */
int main() {
	//construct level class and point adress to global pointer
	static Level level;
	g_level = &level;
	//branch if GLFW isn't initialized and kill the application
	if(!glfwInit()) {
		std::cerr << "GLFW initialization failed.\n";
		std::cin.get();
		return EXIT_FAILURE;
	}
	//set window hints
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//create window
	GLFWwindow* window = glfwCreateWindow(g_level->windowWidth, g_level->windowHeight, "Pac-Man", nullptr, nullptr);
	//set framebuffer size data
	int framebufferWidth = 0, framebufferHeight = 0;
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	//declare framebuffer size callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//setting the OpenGL context to the window
	glfwMakeContextCurrent(window);
	//branch if window isn't created and kill the application
	if(window == nullptr) {
		std::cerr << "GLFW failed on window creation.\n";
		glfwTerminate();
		std::cin.get();
		return EXIT_FAILURE;
	}
	//branch if GLEW isn't initialized and kill the application
	if(glewInit() != GLEW_OK) {
		std::cerr << "GLEW initialization failuare.\n";
		glfwTerminate();
		std::cin.get();
		return EXIT_FAILURE;
	}
	//eanable capture of debug output
	enableDebug();
	//enable MSAA
	glEnable(GL_MULTISAMPLE);
	//enable transparency on texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//load menu texture
	GLuint menuTex = loadTexture("sprite/menu.png", MENU_TEXTURE);
	//set background color black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//construct gameState class
	GameState gameState;
	int levelIndex;
	//menu screen loop
	while(true) {
		//processes all pending events
		glfwPollEvents();
		//for every frame reset background color buffer
		glClear(GL_COLOR_BUFFER_BIT);
		//draw menu screen
		gameState.draw(MENU_TEXTURE);
		//branch if user decide to end program
		if(glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glDeleteTextures(1, &menuTex);
			//end program if 'ESC' key is pressed or the user has closed the window
			return EXIT_SUCCESS;
			//branch if user enters a relevant level index
		} else if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
			levelIndex = 0;
			break;
		} else if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			levelIndex = 1;
			break;
		}
		//go to next buffer
		glfwSwapBuffers(window);
	}
	//reset background color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	//load loading texture
	GLuint loadingTex = loadTexture("sprite/loading.png", LOADING_TEXTURE);
	//draw loading screen
	gameState.draw(LOADING_TEXTURE);
	//go to next buffer
	glfwSwapBuffers(window);
	//delete textures not needed anymore
	glDeleteTextures(1, &menuTex);
	glDeleteTextures(1, &loadingTex);
	//branch if file isn't initialized and kill the application
	if(!g_level->inputData(levelIndex)) {
		std::cerr << "File initialization failed.\n";
		std::cin.get();
		return EXIT_FAILURE;
	}
	//load all relevant textures for the game loop
	GLuint characterTex = loadTexture("sprite/pacman.png", CHARACTER_TEXTURE);
	GLuint numberTex = loadTexture("sprite/number.png", NUMBER_TEXTURE);
	GLuint gameoverTex = loadTexture("sprite/gameover.png", GAMEOVER_TEXTURE);
	GLuint wallTex = loadTexture("sprite/wall.png", WALL_TEXTURE);
	//construct camera class and point adress to global pointer
	static Camera camera(framebufferWidth, framebufferHeight);
	g_camera = &camera;
	//construct maze class
	Maze maze;
	//construct array of scoreboard classes
	std::vector<Score*> scoreboard(4, nullptr);
	for(int i = 0; i < scoreboard.size(); i++) {
		scoreboard[i] = new Score(0, (g_level->gridWidth - 2) - i);
	}
	//construct pacman class
	Pacman pacman;
	//create an array filled with all possible starting positions for ghosts
	int ghostStartRow, ghostStartCol;
	std::vector<std::vector<int>> possibleStartingPos;
	for(int i = 0; i < g_level->gridHeight; i++) {
		for(int j = 0; j < g_level->gridWidth; j++) {
			//branch if ghost can spawn
			if(g_level->grid[i][j] == PELLET) {
				//branch if position is more than 3 rows away from pacman
				if(g_level->pacmanCol - i > 3 || i - g_level->pacmanCol > 3) {
					possibleStartingPos.push_back({ i, j });
				}
			}
		}
	}
	//construct array of ghost classes
	std::vector<Ghost*> ghostArr(1, nullptr);
	for(int i = 0; i < ghostArr.size(); i++) {
		//branch if there are too many ghosts compared to the level and kill the application
		if(ghostArr.size() > possibleStartingPos.size()) {
			std::cerr << "Get ghost position failed.\n";
			glfwTerminate();
			std::cin.get();
			return EXIT_FAILURE;
		}
		//get a random position from array of all possible starting positions
		getGhostPos(possibleStartingPos, ghostStartRow, ghostStartCol);
		ghostArr[i] = new Ghost(ghostStartRow, ghostStartCol);
	}
	//construct pellet class
	Pellet pellet;
	//setup timer
	static double limitFPS = 1.0 / 60.0;
	double lastTime = glfwGetTime(), nowTime = 0, timer = lastTime, deltaTime = 0;
	int counter = 0;
	//loop until user closes window
	while(!glfwWindowShouldClose(window)) {
		//delta time managment
		nowTime = glfwGetTime();
		deltaTime += (nowTime - lastTime) / limitFPS;
		lastTime = nowTime;
		//check if user wants to change gamemode
		if(!g_level->gameover) changeDimension(window);
		//branch if world is in 3D space
		if(g_level->gamemode != TWO_DIMENSIONAL) {
			//enable depth to display 3D space
			glEnable(GL_DEPTH_TEST);
		//branch if world is in 2D space
		} else {
			//disable depth so textures in 2D map are transparent
			glDisable(GL_DEPTH_TEST);
		}
		//processes all pending events
		glfwPollEvents();
		//for every frame reset background color buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//draw maze
		maze.draw();
		//draw scoreboard
		for(int i = 0; i < scoreboard.size(); i++) {
			scoreboard[i]->draw();
			//branch if score has changed and update the scoreboard
			if(g_level->scoreChanged) scoreboard[i]->update(g_level->getScore(i));
		}
		//branch if scoreboard has been updated and reset it
		if(g_level->scoreChanged) g_level->scoreChanged = false;
		//draw pellets
		pellet.draw();
		//draw pacman
		pacman.draw();
		//branch if game isn't over
		if(!g_level->gameover && deltaTime >= 1.0) {
			//translate pacman
			pacman.mov(&pellet);
			//check for user input and change direction accordingly
			pacman.inputDirection(window);
		}
		//draw ghosts
		bool noActiveGhosts = true;
		for(int i = 0; i < ghostArr.size(); i++) {
			//branch if ghost isn't dead
			if(!ghostArr[i]->dead) {
				noActiveGhosts = false;
				ghostArr[i]->draw();
				//branch if game isn't over and translate the ghost
				if(!g_level->gameover && deltaTime >= 1.0) ghostArr[i]->mov();
			}
		}
		//branch if there are no more ghosts on the level and end the game
		if(noActiveGhosts) g_level->gameover = true;
		//branch if the magic effect just started
		if(counter == 0 && g_level->magicEffect) {
			for(int i = 0; i < ghostArr.size(); i++) {
				//branch if ghost isn't dead and change the color
				if(!ghostArr[i]->dead) ghostArr[i]->changeColor(1);
			}
		}
		//branch if game is over and 1 second has gone since game is over and display "GAME OVER" to the screen
		if(g_level->gameover && counter > 0) gameState.draw(GAMEOVER_TEXTURE);
		//branch if there has been one second since game loop started
		if(glfwGetTime() - timer > 1.0f) {
			timer++;
			//branch if game isn't over and pacman has eaten a magic pellet
			if(!g_level->gameover && g_level->magicEffect) {
				counter++;
				//branch if 5 seconds have gone since magic pellet was eaten
				if(counter >= 5) {
					//reset data
					counter = 0;
					g_level->magicEffect = false;
					for(int i = 0; i < ghostArr.size(); i++) {
						//branch if ghost isn't dead and change the color
						if(!ghostArr[i]->dead) ghostArr[i]->changeColor(0);
					}
				}
			}
			//branch if the game is over and increase timer
			if(g_level->gameover) {
				counter++;
				//branch if 2 seconds have gone since game was over and reset counter
				if(counter >= 2) counter = 0;
			}
		}
		//reset delta time 
		if(deltaTime >= 1.0) deltaTime -= 1.0;
		//go to next buffer
		glfwSwapBuffers(window);
		//break loop if 'ESC' key is pressed
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
	}
	//clear memory before ending program
	for(int i = 0; i < scoreboard.size(); i++) {
		delete scoreboard[i];
	}
	scoreboard.clear();
	for(int i = 0; i < ghostArr.size(); i++) {
		delete ghostArr[i];
	}
	ghostArr.clear();
	glUseProgram(0);
	glDeleteTextures(1, &characterTex);
	glDeleteTextures(1, &numberTex);
	glDeleteTextures(1, &gameoverTex);
	glDeleteTextures(1, &wallTex);
	glfwTerminate();
	return EXIT_SUCCESS;
}
