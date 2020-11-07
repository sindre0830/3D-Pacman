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
	glDeleteProgram(shaderProgram3D);
	destroyVAO(vao3D);
}
/**
 * @brief Construct a new Wall:: Wall object
 * 
 */
Maze::Maze(glm::mat4 collectionMatrix) {
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
	//create 3D shader program
	shaderProgram3D = compileShader(wallVertexShader3D, wallFragmentShader3D);

	//generate 3D wall
	arr = genWallCoordinates3D();
	vao3D = genObject(arr, wallSize3D);
    //specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
}
/**
 * @brief Draw object by installing the shader program and binding the VAO to the current rendering state
 * 
 */
void Maze::draw(glm::mat4 collectionMatrix) {
	if(g_level->gamemode == TWO_DIMENSIONAL) {
		glUseProgram(shaderProgram);
		//draw walls
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, (6 * wallSize), GL_UNSIGNED_INT, (const void*)0);
		//draw corners
		glBindVertexArray(cornerVAO);
		glDrawArrays(GL_TRIANGLES, 0, (3 * cornerSize));
	} else {
		auto samplerSlotLocation = glGetUniformLocation(shaderProgram3D, "u_texture");
		glUseProgram(shaderProgram3D);
		glBindVertexArray(vao3D);
		glUniform1i(samplerSlotLocation, 3);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize3D), GL_UNSIGNED_INT, (const void*)0);
	}
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
/**
 * @brief Generate buffer array (x, y) * 4
 * 
 * @param target 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> Maze::genWallCoordinates3D() {
	float Z = g_level->gridElementHeight / 2.f;
	//buffer array
	std::vector<GLfloat> arr;
	//fills in array with coordinates
	for (int i = 0; i < g_level->gridHeight; i++) {
		for (int j = 0; j < g_level->gridWidth; j++) {
			//branch if target is a wall
			if (g_level->grid[i][j] != WALL) {
				//branch if there can be a wall above the target
				if(i + 1 < g_level->gridHeight && g_level->grid[i + 1][j] == WALL) {
					wallSize3D++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y], -Z, 0.f, 1.f,
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y], Z, 0.f, 0.f,
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y], Z, 1.f, 0.f,
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y], -Z, 1.f, 1.f
					});
				}
				//branch if there can be a wall under the target
				if(i - 1 >= 0 && g_level->grid[i - 1][j] == WALL) {
					wallSize3D++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y], -Z, 0.f, 1.f,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y], Z, 0.f, 0.f,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y], Z, 1.f, 0.f,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y], -Z, 1.f, 1.f
					});
				}
				//branch if there can be a wall left of the target
				if(j - 1 >= 0 && g_level->grid[i][j - 1] == WALL) {
					wallSize3D++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y], -Z, 0.f, 1.f,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y], Z, 0.f, 0.f,
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y], Z, 1.f, 0.f,
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y], -Z, 1.f, 1.f
					});
				}
				//branch if there can be a wall right of the target
				if(j + 1 < g_level->gridWidth && g_level->grid[i][j + 1] == WALL) {
					wallSize3D++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y], -Z, 0.f, 1.f,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y], Z, 0.f, 0.f,
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y], Z, 1.f, 0.f,
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y], -Z, 1.f, 1.f
					});
				}
			} else if(g_level->grid[i][j] == WALL) {
				wallSize3D++;
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y], Z, 0.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y], Z, 0.f, 0.f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y], Z, 1.f, 0.f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y], Z, 1.f, 1.f
				});
			}
		}
	}
	return arr;
}
