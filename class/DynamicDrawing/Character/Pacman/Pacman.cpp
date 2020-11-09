/* library */
#include "Pacman.h"
#include "../CharacterShader.h"
#include "../../../../header/dictionary.h"
#include "../../../../header/levelData.h"
#include "../../../../header/function.h"
#include "../../../../header/Camera.h"
/* global data */
extern LevelData *g_level;
extern Camera *g_camera;
/**
 * @brief Destroy the Pacman:: Pacman object
 * 
 */
Pacman::~Pacman() {}
/**
 * @brief Construct a new Pacman:: Pacman object
 * 
 */
Pacman::Pacman() {
	//set model path
	modelPath = "pacman/pacman";
	//set starting postions
    getPosition();
	//set starting direction
	if (g_level->pacmanCol == 0) {
		yTex = 0.5f;
		direction = UP;
	} else if (g_level->pacmanRow == g_level->gridWidth - 1) {
		yTex = 0.25f;
		direction = LEFT;
	} else if (g_level->pacmanCol == g_level->gridHeight - 1) {
		yTex = 0.75f;
		direction = DOWN;
	} else if(g_level->pacmanRow == 0) {
		yTex = 0.0f;
		direction = RIGHT;
	}
    //create VAO
	std::vector<GLfloat> arr = genCoordinates(g_level->pacmanRow, g_level->pacmanCol);
    VAO = genObject(arr, 1);
	//specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	std::cout << "\nxPos: " << g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][X] << "\tyPos: " << g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][Y] << "\n";
	
	camX = g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][X] + g_level->gridElementWidth * 0.5f;
	camY = g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][Y] - g_level->gridElementHeight * 0.5f;
	g_camera->changeDirection(direction);
	if(direction == DOWN) {
		g_camera->changePosition(
			camX,
			camY,
			0.f
		);
	}
    //modelShaderProgram = compileModelShader(characterModelVertexShader, characterModelFragmentShader);
	modelVAO = loadModel("models/pacman/", "pacman.obj", modelSize);
	//sets z value to -1 so we don't see it in first person view
	initialTranslation = glm::vec3(
		g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][X] + g_level->gridElementWidth * 0.5f, 
		g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][Y] - g_level->gridElementHeight * 0.5f, 
		-1.f
	);
}
/**
 * @brief Get starting position in the grid
 * 
 */
void Pacman::getPosition() {
	for (int i = 0; i < g_level->gridHeight; i++) {
		for (int j = 0; j < g_level->gridWidth; j++) {
			if (g_level->grid[i][j] == PACMAN) {
				g_level->pacmanCol = i;
				g_level->pacmanRow = j;
				return;
			}
		}
	}
}
/**
 * @brief Move Pacman to the current direction if possible
 * 
 * @param shader
 */
void Pacman::mov(Pellet &pellet) {
	//branch if pacman can change direction and pacman is moving set change direction to false
	if(changeDirection && counter < speed) changeDirection = false;
	switch (direction) {
		case UP:
			//branch if pacman has reached the next location or if the next location will be a wall
			if(movUp(g_level->pacmanRow, g_level->pacmanCol)) {
				//update position in table
				g_level->pacmanCol++;
				//update camera position
				updateCameraPosition();
				g_camera->changePosition(camX, camY, 0.f);
				//branch if location has a magic pellet or if the next location has a pellet and eat it
				if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == MAGICPELLET) {
					g_level->magicEffect = true;
					eat(pellet);
				} else if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == PELLET) eat(pellet);
			} else if(g_level->grid[g_level->pacmanCol + 1][g_level->pacmanRow] == WALL) {
				changeDirection = true;
			} else g_camera->changePosition(camX, camY + g_level->gridElementHeight / (float)(speed) * (counter), 0.f);
			break;
		case LEFT:
			//branch if pacman has reached the next location or if the next location will be a wall
			if(movLeft(g_level->pacmanRow, g_level->pacmanCol)) {
				//update position in table
				g_level->pacmanRow--;
				//update camera position
				updateCameraPosition();
				g_camera->changePosition(camX, camY, 0.f);
				//branch if location has a magic pellet or if the next location has a pellet and eat it
				if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == MAGICPELLET) {
					g_level->magicEffect = true;
					eat(pellet);
				} else if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == PELLET) eat(pellet);
			} else if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow - 1] == WALL) {
				changeDirection = true;
			} else g_camera->changePosition(camX - g_level->gridElementWidth / (float)(speed) * counter, camY, 0.f);	
			break;
		case DOWN:
			//branch if pacman has reached the next location or if the next location will be a wall
			if(movDown(g_level->pacmanRow, g_level->pacmanCol)) {
				//update position in table
				g_level->pacmanCol--;
				//update camera position
				updateCameraPosition();
				g_camera->changePosition(camX, camY, 0.f);
				//branch if location has a magic pellet or if the next location has a pellet and eat it
				if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == MAGICPELLET) {
					g_level->magicEffect = true;
					eat(pellet);
				} else if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == PELLET) eat(pellet);
			} else if(g_level->grid[g_level->pacmanCol - 1][g_level->pacmanRow] == WALL) {
				changeDirection = true;
			} else g_camera->changePosition(camX, camY - g_level->gridElementHeight / (float)(speed) * counter, 0.f);
			break;
		case RIGHT:
			//branch if pacman has reached the next location or if the next location will be a wall
			if(movRight(g_level->pacmanRow, g_level->pacmanCol)) {
				//update position in table
				g_level->pacmanRow++;
				//update camera position
				updateCameraPosition();
				g_camera->changePosition(camX, camY, 0.f);
				//branch if location has a magic pellet or if the next location has a pellet and eat it
				if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == MAGICPELLET) {
					g_level->magicEffect = true;
					eat(pellet);
				} else if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow] == PELLET) eat(pellet);
			} else if(g_level->grid[g_level->pacmanCol][g_level->pacmanRow + 1] == WALL) {
				changeDirection = true;
			} else g_camera->changePosition(camX + g_level->gridElementWidth / (float)(speed) * counter, camY, 0.f);
			break;
	}
	//animate pacman
	animate();
}
/**
 * @brief Animate pacman
 * 
 */
void Pacman::animate() {
	//animate pacman according to the percentage of movement left till it has reached the next location
	if (counter == speed * 0.25f) {
		translateTex(1.f / 6.f, yTex);
	} else if (counter == speed * 0.5f) {
		translateTex(2.f / 6.f, yTex);
	} else if (counter == speed * 0.75f) {
		translateTex(3.f / 6.f, yTex);
	} else if (counter >= speed) {
		counter = 0;
		changeDirection = true;
		translateTex(0.f, yTex);
	}
}
/**
 * @brief Update score and hide the pellet
 * 
 * @param pellet 
 */
void Pacman::eat(Pellet &pellet) {
	//remove pellet in array
	g_level->grid[g_level->pacmanCol][g_level->pacmanRow] = EMPTY;
	//increment score
	g_level->score++;
	g_level->scoreChanged = true;
	//hide pellet
	pellet.hidePellet(g_level->pacmanCol, g_level->pacmanRow);
	//branch if there isn't any more pellets in the game and end it
	if(g_level->score == g_level->pelletSize) g_level->gameover = true;
}
/**
 * @brief Change direction according to user input
 * 
 * @param window 
 */
void Pacman::inputDirection(GLFWwindow *window) {
	//change direction on key press if pacman has completed a translation and it wont hit a wall
	//if(g_level->gamemode == TWO_DIMENSIONAL) {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && changeDirection && g_level->pacmanCol + 1 < g_level->gridHeight && g_level->grid[g_level->pacmanCol + 1][g_level->pacmanRow] != WALL) {
			direction = UP;
			yTex = 0.5f;
			translateTex(0.f, yTex);
		} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && changeDirection && g_level->pacmanRow - 1 >= 0 && g_level->grid[g_level->pacmanCol][g_level->pacmanRow - 1] != WALL) {
			direction = LEFT;
			yTex = 0.25f;
			translateTex(0.f, yTex);
		} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && changeDirection && g_level->pacmanCol - 1 >= 0 && g_level->grid[g_level->pacmanCol - 1][g_level->pacmanRow] != WALL) {
			direction = DOWN;
			yTex = 0.75f;
			translateTex(0.f, yTex);
		} else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && changeDirection && g_level->pacmanRow + 1 < g_level->gridWidth && g_level->grid[g_level->pacmanCol][g_level->pacmanRow + 1] != WALL) {
			direction = RIGHT;
			yTex = 0.0f;
			translateTex(0.f, yTex);
		}
	/*} else {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && changeDirection && g_level->pacmanCol + 1 < g_level->gridHeight && g_level->grid[g_level->pacmanCol + 1][g_level->pacmanRow] != WALL) {
			if(direction == UP) {

			}
		}
	}*/
	g_camera->changeDirection(direction);
}

void Pacman::updateCameraPosition() {
	camX = g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][X] + g_level->gridElementWidth * 0.5f;
	camY = g_level->gridElement[std::make_pair(g_level->pacmanCol, g_level->pacmanRow)][TOP_LEFT][Y] - g_level->gridElementHeight * 0.5f;
}