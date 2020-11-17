/* library */
#include "header/pacman.h"
#include "shader/character.h"
#include "dictionary.h"
#include "level.h"
#include "functionality.h"
#include "camera.h"
#include <glm/gtc/type_ptr.hpp>
/* global data */
extern Level *g_level;
extern Camera *g_camera;
/**
 * @brief Destroy the Pacman:: Pacman object.
 * 
 */
Pacman::~Pacman() {}
/**
 * @brief Construct a new Pacman:: Pacman object.
 * 
 */
Pacman::Pacman() {
	//set starting postions
    getPosition();
	//set starting direction
	if(g_level->pacmanCol == g_level->colOffset / 2) {
		yTex = 0.5f;
		direction = UP;
	} else if(g_level->pacmanRow == g_level->gridWidth - (g_level->rowOffset / 2) - 1) {
		yTex = 0.25f;
		direction = LEFT;
	} else if(g_level->pacmanCol == g_level->gridHeight - (g_level->colOffset / 2) - 1) {
		yTex = 0.75f;
		direction = DOWN;
	} else if(g_level->pacmanRow == g_level->rowOffset / 2) {
		yTex = 0.0f;
		direction = RIGHT;
	}
	//set initial camera direction
	g_camera->changeDirection(direction);
    //create VAO
	std::vector<GLfloat> arr = genCoordinates(g_level->pacmanRow, g_level->pacmanCol);
    spriteVAO = genObject(arr, 1);
	//specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*)(2 * sizeof(GLfloat)));
	//create model VAO
	modelVAO = loadModel("model/pacman/", "pacman.obj", modelSize);
	//change color to yellow;
	glUseProgram(shaderProgram3D);
	glUniform3fv(glGetUniformLocation(shaderProgram3D, "u_objectColor"), 1, glm::value_ptr(glm::vec3(1.f, 1.f, 0.f)));
	//sets z value to -1 so we don't see it in first person view
	initialTranslation = glm::vec3(
		g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][X] + g_level->gridElementWidth * 0.5f, 
		g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][Y] - g_level->gridElementHeight * 0.5f, 
		-1.f
	);
	//set initial camera positions
	camX = initialTranslation.x;
	camY = initialTranslation.y;
	g_camera->changePosition(camX, camY);
}
/**
 * @brief Get starting position in the grid.
 * 
 */
void Pacman::getPosition() {
	for(int i = 0; i < g_level->gridHeight; i++) {
		for(int j = 0; j < g_level->gridWidth; j++) {
			if(g_level->grid[i][j] == PACMAN) {
				g_level->pacmanCol = i;
				g_level->pacmanRow = j;
				return;
			}
		}
	}
}
/**
 * @brief Move Pacman according to the current direction if possible.
 * 
 * @param shader
 */
void Pacman::mov(Pellet* pellet) {
	bool
		pathUP = g_level->pacmanCol + 1 < g_level->gridHeight && g_level->grid[g_level->pacmanCol + 1][g_level->pacmanRow] != WALL && g_level->grid[g_level->pacmanCol + 1][g_level->pacmanRow] != VOID,
		pathLeft = g_level->pacmanRow - 1 >= 0 && g_level->grid[g_level->pacmanCol][g_level->pacmanRow - 1] != WALL && g_level->grid[g_level->pacmanCol][g_level->pacmanRow - 1] != VOID,
		pathDown = g_level->pacmanCol - 1 >= 0 && g_level->grid[g_level->pacmanCol - 1][g_level->pacmanRow] != WALL && g_level->grid[g_level->pacmanCol - 1][g_level->pacmanRow] != VOID,
		pathRight = g_level->pacmanRow + 1 < g_level->gridWidth && g_level->grid[g_level->pacmanCol][g_level->pacmanRow + 1] != WALL && g_level->grid[g_level->pacmanCol][g_level->pacmanRow + 1] != VOID;
	if(g_level->gamemode == THIRD_PERSON) {
		initialTranslation.z = 0.f;
	} else initialTranslation.z = 10.f;
	//branch if pacman can change direction and pacman is moving set change direction to false
	if(changeDirection && counter < speed) changeDirection = false;
	switch(direction) {
		case UP:
			//branch if pacman has reached the next location or if the next location will be a wall
			if(movUp(g_level->pacmanRow, g_level->pacmanCol)) {
				//update position in table
				g_level->pacmanCol++;
				//update camera position
				updateCameraPosition();
				g_camera->changePosition(camX, camY);
				//branch if location has a magic pellet or if the next location has a pellet and eat it
				if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == MAGICPELLET) {
					g_level->magicEffect = true;
					eat(pellet);
				} else if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == PELLET) eat(pellet);
			} else if(!pathUP) {
				changeDirection = true;
				g_camera->changePosition(camX, camY);
			} else g_camera->changePosition(camX, camY + g_level->gridElementHeight / (float)(speed) * (counter));
			break;
		case LEFT:
			//branch if pacman has reached the next location or if the next location will be a wall
			if(movLeft(g_level->pacmanRow, g_level->pacmanCol)) {
				//update position in table
				g_level->pacmanRow--;
				//update camera position
				updateCameraPosition();
				g_camera->changePosition(camX, camY);
				//branch if location has a magic pellet or if the next location has a pellet and eat it
				if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == MAGICPELLET) {
					g_level->magicEffect = true;
					eat(pellet);
				} else if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == PELLET) eat(pellet);
			} else if(!pathLeft) {
				changeDirection = true;
				g_camera->changePosition(camX, camY);
			} else g_camera->changePosition(camX - g_level->gridElementWidth / (float)(speed) * counter, camY);	
			break;
		case DOWN:
			//branch if pacman has reached the next location or if the next location will be a wall
			if(movDown(g_level->pacmanRow, g_level->pacmanCol)) {
				//update position in table
				g_level->pacmanCol--;
				//update camera position
				updateCameraPosition();
				g_camera->changePosition(camX, camY);
				//branch if location has a magic pellet or if the next location has a pellet and eat it
				if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == MAGICPELLET) {
					g_level->magicEffect = true;
					eat(pellet);
				} else if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == PELLET) eat(pellet);
			} else if(!pathDown) {
				changeDirection = true;
				g_camera->changePosition(camX, camY);
			} else g_camera->changePosition(camX, camY - g_level->gridElementHeight / (float)(speed) * counter);
			break;
		case RIGHT:
			//branch if pacman has reached the next location or if the next location will be a wall
			if(movRight(g_level->pacmanRow, g_level->pacmanCol)) {
				//update position in table
				g_level->pacmanRow++;
				//update camera position
				updateCameraPosition();
				g_camera->changePosition(camX, camY);
				//branch if location has a magic pellet or if the next location has a pellet and eat it
				if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == MAGICPELLET) {
					g_level->magicEffect = true;
					eat(pellet);
				} else if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == PELLET) eat(pellet);
			} else if(!pathRight) {
				changeDirection = true;
				g_camera->changePosition(camX, camY);
			} else g_camera->changePosition(camX + g_level->gridElementWidth / (float)(speed) * counter, camY);
			break;
	}
	//animate pacman
	animate();
}
/**
 * @brief Animate pacman.
 * 
 */
void Pacman::animate() {
	//animate pacman according to the percentage of movement left till it has reached the next location
	if(counter == speed * 0.25f) {
		translateTex(1.f / 6.f, yTex);
	} else if(counter == speed * 0.5f) {
		translateTex(2.f / 6.f, yTex);
	} else if(counter == speed * 0.75f) {
		translateTex(3.f / 6.f, yTex);
	} else if(counter >= speed) {
		counter = 0;
		changeDirection = true;
		translateTex(0.f, yTex);
	}
}
/**
 * @brief Update score and hide the pellet.
 * 
 * @param pellet 
 */
void Pacman::eat(Pellet* pellet) {
	//remove pellet in array
	g_level->grid[g_level->pacmanCol][g_level->pacmanRow] = EMPTY;
	//increment score
	g_level->score++;
	g_level->scoreChanged = true;
	//hide pellet
	pellet->hidePellet(g_level->pacmanCol, g_level->pacmanRow);
	//branch if there isn't any more pellets in the game and end it
	if(g_level->score == g_level->pelletSize) g_level->gameover = true;
}
/**
 * @brief Change direction according to user input.
 * 
 * @param window 
 */
void Pacman::inputDirection(GLFWwindow *window) {
	bool
		pathUP = g_level->pacmanCol + 1 < g_level->gridHeight && g_level->grid[g_level->pacmanCol + 1][g_level->pacmanRow] != WALL && g_level->grid[g_level->pacmanCol + 1][g_level->pacmanRow] != VOID,
		pathLeft = g_level->pacmanRow - 1 >= 0 && g_level->grid[g_level->pacmanCol][g_level->pacmanRow - 1] != WALL && g_level->grid[g_level->pacmanCol][g_level->pacmanRow - 1] != VOID,
		pathDown = g_level->pacmanCol - 1 >= 0 && g_level->grid[g_level->pacmanCol - 1][g_level->pacmanRow] != WALL && g_level->grid[g_level->pacmanCol - 1][g_level->pacmanRow] != VOID,
		pathRight = g_level->pacmanRow + 1 < g_level->gridWidth && g_level->grid[g_level->pacmanCol][g_level->pacmanRow + 1] != WALL && g_level->grid[g_level->pacmanCol][g_level->pacmanRow + 1] != VOID;
	//change direction on key press if pacman has completed a translation and it wont hit a wall
	if(changeDirection) {
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			if(pathUP && g_level->gamemode != FIRST_PERSON) {
				direction = UP;
				yTex = 0.5f;
				translateTex(0.f, yTex);	
			}
		} else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			if(g_level->gamemode == FIRST_PERSON) {
				if(direction == UP && pathLeft) {
					direction = LEFT;
					yTex = 0.25f;
					translateTex(0.f, yTex);
				} else if(direction == LEFT && pathDown) {
					direction = DOWN;
					yTex = 0.75f;
					translateTex(0.f, yTex);
				} else if(direction == DOWN && pathRight) {
					direction = RIGHT;
					yTex = 0.0f;
					translateTex(0.f, yTex);
				} else if(direction == RIGHT && pathUP) {
					direction = UP;
					yTex = 0.5f;
					translateTex(0.f, yTex);
				}
			} else if(pathLeft) {
				direction = LEFT;
				yTex = 0.25f;
				translateTex(0.f, yTex);
			}
		} else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			if(g_level->gamemode == FIRST_PERSON) {
				if(direction == UP && pathDown) {
					direction = DOWN;
					yTex = 0.75f;
					translateTex(0.f, yTex);
				} else if(direction == LEFT && pathRight) {
					direction = RIGHT;
					yTex = 0.0f;
					translateTex(0.f, yTex);
				} else if(direction == DOWN && pathUP) {
					direction = UP;
					yTex = 0.5f;
					translateTex(0.f, yTex);
				} else if(direction == RIGHT && pathLeft) {
					direction = LEFT;
					yTex = 0.25f;
					translateTex(0.f, yTex);
				}
			} else if(pathDown) {
				direction = DOWN;
				yTex = 0.75f;
				translateTex(0.f, yTex);
			}
		} else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if(g_level->gamemode == FIRST_PERSON) {
				if(direction == UP && pathRight) {
					direction = RIGHT;
					yTex = 0.0f;
					translateTex(0.f, yTex);
				} else if(direction == LEFT && pathUP) {
					direction = UP;
					yTex = 0.5f;
					translateTex(0.f, yTex);
				} else if(direction == DOWN && pathLeft) {
					direction = LEFT;
					yTex = 0.25f;
					translateTex(0.f, yTex);
				} else if(direction == RIGHT && pathDown) {
					direction = DOWN;
					yTex = 0.75f;
					translateTex(0.f, yTex);
				}
			} else if(pathRight) {
				direction = RIGHT;
				yTex = 0.0f;
				translateTex(0.f, yTex);
			}
		}	
	}
	g_camera->changeDirection(direction);
}
/**
 * @brief Update camera position values.
 * 
 */
void Pacman::updateCameraPosition() {
	camX = g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][X] + g_level->gridElementWidth * 0.5f;
	camY = g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][Y] - g_level->gridElementHeight * 0.5f;
}