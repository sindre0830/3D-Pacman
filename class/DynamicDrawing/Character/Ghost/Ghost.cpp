/* library */
#include "Ghost.h"
#include "../../../../header/dictionary.h"
#include "../../../../header/levelData.h"
#include "../../../../header/function.h"
/* global data */
extern LevelData *g_level;
/**
 * @brief Destroy the Ghost:: Ghost object
 * 
 */
Ghost::~Ghost() {}
/**
 * @brief Construct a new Ghost:: Ghost object
 * 
 * @param row 
 * @param col 
 */
Ghost::Ghost(const int row, const int col) {
	//set starting postions
	rowPos = row;
	colPos = col;
	//set random direction
	findRandomPath();
    //create VAO
	std::vector<GLfloat> arr = genCoordinates(rowPos, colPos);
    VAO = genObject(arr, 1);
	//specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	//translate texture to show ghost
	translateTex(4.0f / 6.0f, yTex);

    //modelShaderProgram = compileModelShader(characterModelVertexShader, characterModelFragmentShader);
	modelVAO = loadModel("models/ghost/ghost/", "ghost.obj", modelSize);
	initialTranslation = glm::vec3(g_level->gridElement[std::make_pair(col, row)][TOP_LEFT][X] + g_level->gridElementWidth * 0.5f, g_level->gridElement[std::make_pair(col, row)][TOP_LEFT][Y] - g_level->gridElementHeight * 0.5f, 0.f);
}
/**
 * @brief Finds aggresive path to Pacman unless magic effect is active
 * 
 */
void Ghost::pathfinding() {
	//branch if magic pellet effect isn't active or find a random path
	if(!g_level->magicEffect) {
		//branch if path isn't opposite of current direction and there isn't a wall
		if(direction != DOWN && g_level->grid[colPos + 1][rowPos] != WALL && colPos < g_level->pacmanCol) {
			direction = UP;
		} else if(direction != UP && g_level->grid[colPos - 1][rowPos] != WALL && colPos > g_level->pacmanCol) {
			direction = DOWN;
		} else if(direction != RIGHT && g_level->grid[colPos][rowPos - 1] != WALL && rowPos > g_level->pacmanRow) {
			direction = LEFT;
		} else if(direction != LEFT && g_level->grid[colPos][rowPos + 1] != WALL && rowPos < g_level->pacmanRow) {
			direction = RIGHT;
		} else findRandomPath();
	} else findRandomPath();
}
/**
 * @brief Finds random path
 * 
 */
void Ghost::findRandomPath() {
	int index = 0;
	//store all possible directions in an array
	std::vector<int> possiblePaths;
	//branch if path isn't opposite of current direction and there isn't a wall
	if(direction != DOWN && g_level->grid[colPos + 1][rowPos] != WALL) possiblePaths.push_back(UP);
	if(direction != RIGHT && g_level->grid[colPos][rowPos - 1] != WALL) possiblePaths.push_back(LEFT);
	if(direction != UP && g_level->grid[colPos - 1][rowPos] != WALL) possiblePaths.push_back(DOWN);
	if(direction != LEFT && g_level->grid[colPos][rowPos + 1] != WALL) possiblePaths.push_back(RIGHT);
	//branch if more then one direction is possible and shuffle the array
	if(possiblePaths.size() > 1) index = randomIndex(0, possiblePaths.size() - 1);
	//set direction
	direction = possiblePaths[index];
}
/**
 * @brief Move ghost to the current direction until collision
 * 
 * @param shader
 */
void Ghost::mov() {
	//check if ghost has collided with pacman
	checkCoalition(rowPos, colPos);
	switch (direction) {
		case UP:
			//face up
			yTex = 0.5f;
			//branch if ghost was able to move and increase counter or check for coalition 1/4 into the translation
			if(movUp(rowPos, colPos)) {
				//update the ghost position in array
				colPos++;
				//check if ghost has collided with pacman
				checkCoalition(rowPos, colPos);
				//branch if ghost isn't going to teleport and find a path
				if(colPos + 1 < g_level->gridHeight) pathfinding();
			} else if(counter >= speed / 4) checkCoalition(rowPos, colPos + 1);
			break;
		case LEFT:
			//face left
			yTex = 0.25f;
			//branch if ghost was able to move and increase counter or check for coalition 1/4 into the translation
			if(movLeft(rowPos, colPos)) {
				//update the ghost position in array
				rowPos--;
				//check if ghost has collided with pacman
				checkCoalition(rowPos, colPos);
				//branch if ghost isn't going to teleport and find a path
				if(rowPos - 1 >= 0) pathfinding();
			} else if(counter >= speed / 4) checkCoalition(rowPos - 1, colPos);
			break;
		case DOWN:
			//face down
			yTex = 0.75f;
			//branch if ghost was able to move and increase counter or check for coalition 1/4 into the translation
			if(movDown(rowPos, colPos)) {
				//update the ghost position in array
				colPos--;
				//check if ghost has collided with pacman
				checkCoalition(rowPos, colPos);
				//branch if ghost isn't going to teleport and find a path
				if(colPos - 1 >= 0) pathfinding();
			} else if(counter >= speed / 4) checkCoalition(rowPos, colPos - 1);
			break;
		case RIGHT:
			//face right
			yTex = 0.0f;
			//branch if ghost was able to move and increase counter or check for coalition 1/4 into the translation
			if(movRight(rowPos, colPos)) {
				//update the ghost position in array
				rowPos++;
				//check if ghost has collided with pacman
				checkCoalition(rowPos, colPos);
				//branch if ghost isn't going to teleport and find a path
				if(rowPos + 1 < g_level->gridWidth) pathfinding();
			} else if(counter >= speed / 4) checkCoalition(rowPos + 1, colPos);
			break;
	}
	//animate ghost
	animate();
}
/**
 * @brief Check if ghost has collided with pacman and end the game
 * 
 * @param row 
 * @param col 
 */
void Ghost::checkCoalition(const int row, const int col) {
	//branch if ghost and pacman has the same location
	if(row == g_level->pacmanRow && col == g_level->pacmanCol) {
		//branch if magic effect is active or end game
		if(g_level->magicEffect) {
			dead = true;
		} else g_level->gameover = true;
	}
}
/**
 * @brief Animate ghost
 * 
 */
void Ghost::animate() {
	//animate ghost according to the percentage of movement left till it has reached the next location
	if (counter == speed * 0.25f) {
		translateTex(4.0f / 6.0f, yTex);
	} else if (counter == speed * 0.5f) {
		translateTex(5.0f / 6.0f, yTex);
	} else if (counter == speed * 0.75f) {
		translateTex(4.0f / 6.0f, yTex);
	} else if (counter >= speed) {
		counter = 0;
		translateTex(5.0f / 6.0f, yTex);
	}
}
/**
 * @brief Change color if magic effect is active
 * 
 * @param flag 
 */
void Ghost::changeColor(const int flag) {
	//get uniform to transform
	GLuint color = glGetUniformLocation(shaderProgram, "u_changeColor");
	//send data to uniform
	glUniform1i(color, flag);
	//update to the new color
	draw(glm::mat4(1.f));
}
