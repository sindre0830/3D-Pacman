/* library */
#include "Maze.h"
#include "MazeShader.h"
#include "../../../header/dictionary.h"
#include "../../../header/levelData.h"
/* global data */
extern LevelData *g_level;
/**
 * @brief Destroy the Wall:: Wall object
 * 
 */
Maze::~Maze() {
    destroyVAO(cornerVAO);
}
/**
 * @brief Construct a new Wall:: Wall object
 * 
 */
Maze::Maze() {
	//create shader program
    shaderProgram = compileShader(wallVertexShader, wallFragmentShader);
	//generate wall VAO
	std::vector<GLfloat> arr = genWallCoordinates();
    VAO = genObject(arr, wallSize);
	//set the vertex attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLuint), (const void*)0);
	glEnableVertexAttribArray(0);
	//generate corner VAO
	cornerVAO = genCornerVAO();
	//set the vertex attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLuint), (const void*)0);
	glEnableVertexAttribArray(0);
}
/**
 * @brief Draw object by installing the shader program and binding the VAO to the current rendering state
 * 
 */
void Maze::draw() {
	glUseProgram(shaderProgram);
	//draw walls
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (6 * wallSize), GL_UNSIGNED_INT, (const void*)0);
	//draw corners
	glBindVertexArray(cornerVAO);
	glDrawArrays(GL_TRIANGLES, 0, (3 * cornerSize));
}
/**
 * @brief Generate buffer array (x, y) * 4
 * 
 * @param target 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> Maze::genWallCoordinates() {
	float
		xResize = g_level->gridElementWidth / 1.2f,
		yResize = g_level->gridElementHeight / 1.2f;
	//buffer array
	std::vector<GLfloat> arr;
	//fills in array with coordinates
	for (int i = 0; i < g_level->gridHeight; i++) {
		for (int j = 0; j < g_level->gridWidth; j++) {
			//branch if target is a wall
			if (g_level->grid[i][j] == WALL) {
				//branch if there can be a wall above the target
				if(i + 1 < g_level->gridHeight && g_level->grid[i + 1][j] != WALL) {
					wallSize++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y],
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yResize,
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y]
					});
				}
				//branch if there can be a wall under the target
				if(i - 1 >= 0 && g_level->grid[i - 1][j] != WALL) {
					wallSize++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yResize,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y],
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y],
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize
					});
				}
				//branch if there can be a wall left of the target
				if(j - 1 >= 0 && g_level->grid[i][j - 1] != WALL) {
					wallSize++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y],
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y],
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y],
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y]
					});
				}
				//branch if there can be a wall right of the target
				if(j + 1 < g_level->gridWidth && g_level->grid[i][j + 1] != WALL) {
					wallSize++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y],
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y],
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y],
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y]
					});
				}
			}
		}
	}
	return arr;
}
/**
 * @brief Generate buffer array (x, y) * 3
 * 
 * @param target 
 * @return std::vector<GLfloat> 
 */
GLuint Maze::genCornerVAO() {
	float
		//resize corner acording to size of wall
		xResize = (g_level->gridElementWidth / 1.2f) / 5.f,
		yResize = (g_level->gridElementHeight / 1.2f) / 5.f;
	//buffer array
	std::vector<GLfloat> arr;
	//fills in array with coordinates
	for (int i = 0; i < g_level->gridHeight; i++) {
		for (int j = 0; j < g_level->gridWidth; j++) {
			//branch if target is a wall
			if (g_level->grid[i][j] == WALL) {
				//branch if there can be a corner top left of the target
				if(i + 1 < g_level->gridHeight && g_level->grid[i + 1][j] != WALL && j - 1 >= 0 && g_level->grid[i + 1][j - 1] == WALL) {
					cornerSize++;
					arr.insert(arr.end(), {
						//top left coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y],
						//bottom right coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yResize,
						//top right coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y]
					});
				}
				//branch if there can be a corner bottom left of the target
				if(i - 1 >= 0 && g_level->grid[i - 1][j] != WALL && j - 1 >= 0 && g_level->grid[i - 1][j - 1] == WALL) {
					cornerSize++;
					arr.insert(arr.end(), {
						//top left coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y],
						//bottom right coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize,
						//top right coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y]
					});
				}
				//branch if there can be a corner bottom right of the target
				if(i - 1 >= 0 && g_level->grid[i - 1][j] != WALL && j + 1 < g_level->gridWidth && g_level->grid[i - 1][j + 1] == WALL) {
					cornerSize++;
					arr.insert(arr.end(), {
						//top left coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y],
						//bottom left coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yResize,
						//top right coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y]
					});
				}
				//branch if there can be a corner top right of the target
				if(i + 1 < g_level->gridHeight && g_level->grid[i + 1][j] != WALL && j + 1 < g_level->gridWidth && g_level->grid[i + 1][j + 1] == WALL) {
					cornerSize++;
					arr.insert(arr.end(), {
						//top left coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y],
						//bottom left coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize,
						//top right coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y]
					});
				}
			}
		}
	}
	//create and bind the vertex array object
	GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
	//create the vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);
	//set vbo to arr data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, arr.size() * sizeof(GLfloat), arr.data(), GL_STATIC_DRAW);
	return vao;
}
