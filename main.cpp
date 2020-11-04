/**
 * Assignment 1 - Pacman
 * Fully functional Pacman game
 *
 * @author Sindre Eiklid (sindreik@stud.ntnu.no)
 * @author Casper Melhus
 * @author Brage Heimly N�ss
 * 
 * @date 2020/10/21
 */
/* library */
#include "header/dictionary.h"
#include "header/levelData.h"
#include "header/function.h"
#include "class/StaticDrawing/GameState/GameState.h"
#include "class/DynamicDrawing/Score/Score.h"
#include "class/StaticDrawing/Maze/Maze.h"
#include "class/StaticDrawing/Pellet/Pellet.h"
#include "class/DynamicDrawing/Character/Pacman/Pacman.h"
#include "class/DynamicDrawing/Character/Ghost/Ghost.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
/* global data */
LevelData *g_level;
float yaw, pitch, lastX, lastY;
bool firstMouse;
glm::vec3 camFront;
/**
 * Main program.
 */
int main() {
	static LevelData level;
	g_level = &level;
	//branch if file isn't initialized and kill the application
	if (!g_level->inputData()) {
		std::cerr << "File initialization failed.\n";
		std::cin.get();
		return EXIT_FAILURE;
	}
	//branch if GLFW isn't initialized and kill the application
	if(!glfwInit()) {
		std::cerr << "GLFW initialization failed.\n";
		std::cin.get();
		return EXIT_FAILURE;
	}
	//create framebuffer size variables
	int framebufferWidth = 0, framebufferHeight = 0;
	//set window hints
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//create window
	GLFWwindow *window = glfwCreateWindow(g_level->windowWidth, g_level->windowHeight, "Pac-Man", nullptr, nullptr);
	//set the framebuffer size variables
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//setting the OpenGL context to the window
	glfwMakeContextCurrent(window);
	//allow capture of cursor and focus it on the middle while hiding the icon
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//branch if window isn't created and kill the application
	if (window == nullptr) {
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

	//setup rotate
	glm::mat4 modelMatrix(1.f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
	/*modelMatrix = glm::rotate(modelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f));*/
	//setup camera
	glm::vec3 camPos(0.f, 0.f, 3.f);
	camFront = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 camUp(0.f, 1.f, 0.f);

	glm::mat4 viewMatrix(1.f);
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);

	float camSpeed = 0.05f;

	yaw = -90.0f,
	pitch = 0.f;
	
	lastX = g_level->windowWidth / 2.f;
	lastY = g_level->windowHeight / 2.f;

	firstMouse = true;

	glfwSetCursorPosCallback(window, mouse_callback);

	float fov = 45.f;
	float nearPlane = 0.1f;
	float farPlane = 100.f;
	glm::mat4 projectionMatrix(1.f);
	projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);
	projectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
	
	//construct maze
	Maze maze(projectionMatrix);
	//construct array of scoreboard classes
	std::vector<Score*> scoreboard(4, nullptr);
	for(int i = 0; i < scoreboard.size(); i++) {
		scoreboard[i] = new Score(0, (g_level->gridWidth - 2) - i);
	}
	//construct gameState
	GameState gameState;
	//construct pacman
	Pacman pacman;
	//create an array filled with all possible starting positions for ghosts
	int ghostStartRow, ghostStartCol;
	std::vector<std::vector<int>> possibleStartingPos;
	for(int i = 0; i < g_level->gridHeight; i++) {
		for(int j = 0; j < g_level->gridWidth; j++) {
			//branch if ghost can spawn
			if(g_level->grid[i][j] == PELLET) possibleStartingPos.push_back({i, j});
		}
	}
	//construct array of ghost classes
	std::vector<Ghost*> ghostArr(4, nullptr);
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
	//construct pellets
	Pellet pellet;
	//enable depth
	glEnable(GL_DEPTH_TEST);
	//enable MSAA
	glEnable(GL_MULTISAMPLE);
	//enable transparency on texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //load the texture, create OpenGL texture, and bind it to the current context
	GLuint characterTex = loadTexture("sprite/pacman.png", 0);
    GLuint numberTex = loadTexture("sprite/number.png", 1);
    GLuint gameoverTex = loadTexture("sprite/gameover.png", 2);
	GLuint wallTex = loadTexture("sprite/wall.png", 3);
	//set background color black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//setup timer
	static double limitFPS = 1.0 / 60.0;
    double lastTime = glfwGetTime(), nowTime = 0, timer = lastTime;
    double deltaTime = 0;
	int counter = 0;

	g_level->gamemode = FIRST_PERSON;
	//loop until user closes window
	while(!glfwWindowShouldClose(window)) {
		//delta time managment
		nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / limitFPS;
        lastTime = nowTime;
		//processes all pending events
		glfwPollEvents();
		//for every frame reset background color buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//set modelmatrix
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, 0.f));
		//modelMatrix = glm::rotate(modelMatrix, glm::radians(0.f), glm::vec3(1.f, 1.f, 1.f));
		//modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f));
		
		processInput(window, deltaTime, camPos, camFront, camSpeed, camUp);
		viewMatrix = glm::mat4(1.f);
		viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
		/*camX = sin(glfwGetTime()) * radius;
		camZ = cos(glfwGetTime()) * radius;
		glm::mat4 viewMatrix(1.f);
		viewMatrix = glm::lookAt(
			glm::vec3(camX, 0.0, camZ), 
			glm::vec3(0.0, 0.0, 0.0), 
			glm::vec3(0.0, 1.0, 0.0)
		);*/
		//update framebuffer size
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		projectionMatrix = glm::mat4(1.f);
		projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);
		projectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
		//draw maze
		maze.draw(projectionMatrix);
		//branch if gamemode is 2D, else if gamemode is 3D, and call their respected game loops
		if(g_level->gamemode == TWO_DIMENSIONAL) {
			gameloop2D(window, &pellet, &pacman, ghostArr, deltaTime, counter);
		} else gameloop3D(window, &pellet, &pacman, ghostArr, deltaTime, counter);
		//draw scoreboard
		for(int i = 0; i < scoreboard.size(); i++) {
			scoreboard[i]->draw();
			//branch if score has changed and update the scoreboard
			if (g_level->scoreChanged) scoreboard[i]->update(g_level->getScore(i));
		}
		//branch if scoreboard has been updated and reset it
		if(g_level->scoreChanged) g_level->scoreChanged = false;
		//branch if the magic effect just started
		if(counter == 0 && g_level->magicEffect) {
			for(int i = 0; i < ghostArr.size(); i++) {
				//branch if ghost isn't dead and change the color
				if(!ghostArr[i]->dead) ghostArr[i]->changeColor(1);
			}
		}
		//branch if game is over and 1 second has gone since game is over and display "GAME OVER" to the screen
		if(g_level->gameover && counter > 0) gameState.draw();
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
			//branch if the game is over
			if(g_level->gameover) {
				counter++;
				//branch if 2 seconds have gone since game was over
				if(counter >= 2) {
					//reset data
					counter = 0;
				}
			}
		}
		//reset delta time 
		if (!g_level->gameover && deltaTime >= 1.0) deltaTime -= 1.0;
		//go to next buffer
		glfwSwapBuffers(window);
		//dynamic resizing
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			int widthDifference = width - g_level->windowWidth;
			int heightDifference = height - g_level->windowHeight;
			if(widthDifference > 0 && heightDifference > 0) {
				glViewport((widthDifference / 2) - (heightDifference / 2), 0, (width - widthDifference) + heightDifference, height);
			} else if(widthDifference < 0 && heightDifference < 0) {
				glViewport(-(heightDifference / 2), -(widthDifference / 2), (width + heightDifference), (height + widthDifference)); //wrong
			} else if(widthDifference > 0 && heightDifference < 0) {
				glViewport((widthDifference / 2) - (heightDifference / 2), 0, (width - widthDifference) + heightDifference, height);
			} else if(widthDifference < 0 && heightDifference > 0) {
				glViewport(0, -(widthDifference / 2) + (heightDifference / 2), width, (height + widthDifference) - heightDifference);
			} else {
				if(widthDifference > 0) {
					glViewport((widthDifference / 2), 0, (width - widthDifference), height);
				} else if(widthDifference < 0) {
					glViewport(0, -(widthDifference / 2), width, (height + widthDifference));
				}
				if(heightDifference > 0) {
					glViewport(0, (heightDifference / 2), width, (height - heightDifference));
				} else if(heightDifference < 0) {
					glViewport(-(heightDifference / 2), 0, (width + heightDifference), height);
				}
			}
			
		});
		//break loop if 'ESC' key is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
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
