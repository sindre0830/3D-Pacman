/* library */
#include "header/maze.h"
#include "shader/maze.h"
#include "dictionary.h"
#include "level.h"
#include "camera.h"
#include "functionality.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
/* global data */
extern Level *g_level;
extern Camera *g_camera;
/**
 * @brief Destroy the Maze:: Maze object.
 * 
 */
Maze::~Maze() {
	glDeleteProgram(shaderProgram2D);
	glDeleteProgram(shaderProgram3D);
    destroyVAO(wallVAO);
    destroyVAO(cornerVAO);
	destroyVAO(modelVAO);
}
/**
 * @brief Construct a new Maze:: Maze object.
 * 
 */
Maze::Maze() {
	//create shader program
    shaderProgram2D = compileShader(wallVertexShader, wallFragmentShader);
	//generate wall VAO
	std::vector<GLfloat> arr = genWallCoordinates();
    wallVAO = genObject(arr, wallSize);
	//set the vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(2 * sizeof(GLfloat)));
	//generate corner VAO
	cornerVAO = genCornerVAO();
	//set the vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(2 * sizeof(GLfloat)));
	//create 3D shader program
	shaderProgram3D = compileShader(wallVertexShader3D, wallFragmentShader3D);
	//generate 3D wall
	arr = genWallCoordinates3D();
	modelVAO = genObject(arr, wallSize3D);
    //specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(3 * sizeof(GLfloat)));
}
/**
 * @brief Draw object by activating the shader program and binding the VAO and uniforms to the current rendering state.
 * 
 */
void Maze::draw() {
	glUseProgram(shaderProgram2D);
	glBindVertexArray(wallVAO);
	if(g_level->gamemode == TWO_DIMENSIONAL) {
		//draw walls
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram2D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
		glDrawElements(GL_TRIANGLES, (6 * wallSize), GL_UNSIGNED_INT, (const void*)(0));
		//draw corners
		glBindVertexArray(cornerVAO);
		glDrawArrays(GL_TRIANGLES, 0, (3 * cornerSize));
	} else {
		//draw minimap
		glm::mat4 modelMatrix = getMinimapModelMatrix();
		//draw walls
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram2D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize), GL_UNSIGNED_INT, (const void*)(0));
		//draw corners
		glBindVertexArray(cornerVAO);
		glDrawArrays(GL_TRIANGLES, 0, (3 * cornerSize));
		//draw in 3D space
		glUseProgram(shaderProgram3D);
		glBindVertexArray(modelVAO);
		//set texture
		glUniform1i(glGetUniformLocation(shaderProgram3D, "u_texture"), WALL_TEXTURE);
		//draw main scene
		glm::mat4 collectionMatrix = g_camera->projectionMatrix * g_camera->viewMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize3D), GL_UNSIGNED_INT, (const void*)(0));
		//mirror scene top
		modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 2.f + (g_level->colOffset * g_level->gridElementHeight), 0.f));
		collectionMatrix = g_camera->projectionMatrix * g_camera->viewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize3D), GL_UNSIGNED_INT, (const void*)(0));
		//mirror scene left
		modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(-2.f + (g_level->rowOffset * g_level->gridElementWidth), 0.f, 0.f));
		collectionMatrix = g_camera->projectionMatrix * g_camera->viewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize3D), GL_UNSIGNED_INT, (const void*)(0));
		//mirror scene bottom
		modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -2.f - (g_level->colOffset * g_level->gridElementHeight), 0.f));
		collectionMatrix = g_camera->projectionMatrix * g_camera->viewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize3D), GL_UNSIGNED_INT, (const void*)(0));
		//mirror scene right
		modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(2.f - (g_level->rowOffset * g_level->gridElementWidth), 0.f, 0.f));
		collectionMatrix = g_camera->projectionMatrix * g_camera->viewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize3D), GL_UNSIGNED_INT, (const void*)(0));
		//mirror scene top left
		modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(-2.f + (g_level->rowOffset * g_level->gridElementWidth), 2.f + (g_level->colOffset * g_level->gridElementHeight), 0.f));
		collectionMatrix = g_camera->projectionMatrix * g_camera->viewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize3D), GL_UNSIGNED_INT, (const void*)(0));
		//mirror scene bottom left
		modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(-2.f + (g_level->rowOffset * g_level->gridElementWidth), -2.f - (g_level->colOffset * g_level->gridElementHeight), 0.f));
		collectionMatrix = g_camera->projectionMatrix * g_camera->viewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize3D), GL_UNSIGNED_INT, (const void*)(0));
		//mirror scene bottom right
		modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(2.f - (g_level->rowOffset * g_level->gridElementWidth), -2.f - (g_level->colOffset * g_level->gridElementHeight), 0.f));
		collectionMatrix = g_camera->projectionMatrix * g_camera->viewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize3D), GL_UNSIGNED_INT, (const void*)(0));
		//mirror scene top right
		modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(2.f - (g_level->rowOffset * g_level->gridElementWidth), 2.f + (g_level->colOffset * g_level->gridElementHeight), 0.f));
		collectionMatrix = g_camera->projectionMatrix * g_camera->viewMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * wallSize3D), GL_UNSIGNED_INT, (const void*)(0));
	}
}
/**
 * @brief Generate buffer array (x, y, z, r, g, b) * 4.
 * 
 * @param target 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> Maze::genWallCoordinates() {
	float
		xResize = g_level->gridElementWidth / 1.2f,
		yResize = g_level->gridElementHeight / 1.2f;
	//color vector
	glm::vec3 
		wallColor(0.09f, 0.09f, 0.4f),
		floorColor(0.f, 0.f, 0.f);
	//buffer array
	std::vector<GLfloat> arr;
	//fills in array with coordinates
	for(int i = 0; i < g_level->gridHeight; i++) {
		for(int j = 0; j < g_level->gridWidth; j++) {
			//branch if target is a wall
			if(g_level->grid[i][j] == WALL) {
				//branch if there can be a wall above the target
				if(i + 1 < g_level->gridHeight && g_level->grid[i + 1][j] != WALL && g_level->grid[i + 1][j] != VOID) {
					wallSize++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y],
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize,
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yResize,
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y],
						wallColor.x, wallColor.y, wallColor.z
					});
				}
				//branch if there can be a wall under the target
				if(i - 1 >= 0 && g_level->grid[i - 1][j] != WALL && g_level->grid[i - 1][j] != VOID) {
					wallSize++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yResize,
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y],
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y],
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize,
						wallColor.x, wallColor.y, wallColor.z
					});
				}
				//branch if there can be a wall left of the target
				if(j - 1 >= 0 && g_level->grid[i][j - 1] != WALL && g_level->grid[i][j - 1] != VOID) {
					wallSize++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y],
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y],
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y],
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y],
						wallColor.x, wallColor.y, wallColor.z
					});
				}
				//branch if there can be a wall right of the target
				if(j + 1 < g_level->gridWidth && g_level->grid[i][j + 1] != WALL && g_level->grid[i][j + 1] != VOID) {
					wallSize++;
					arr.insert(arr.end(), {
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y],
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y],
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y],
						wallColor.x, wallColor.y, wallColor.z,
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y],
						wallColor.x, wallColor.y, wallColor.z
					});
				}
			} else if(g_level->grid[i][j] != VOID) {
				wallSize++;
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y],
					floorColor.x, floorColor.y, floorColor.z,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y],
					floorColor.x, floorColor.y, floorColor.z,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y],
					floorColor.x, floorColor.y, floorColor.z,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y],
					floorColor.x, floorColor.y, floorColor.z
				});
			}
		}
	}
	return arr;
}
/**
 * @brief Generate buffer array (x, y, z, r, g, b) * 3.
 * 
 * @param target 
 * @return std::vector<GLfloat> 
 */
GLuint Maze::genCornerVAO() {
	float
		//resize corner acording to size of wall
		xResize = (g_level->gridElementWidth / 1.2f) / 5.f,
		yResize = (g_level->gridElementHeight / 1.2f) / 5.f;
	//color vector
	glm::vec3 colorBlue(0.09f, 0.09f, 0.4f);
	//buffer array
	std::vector<GLfloat> arr;
	//fills in array with coordinates
	for(int i = 0; i < g_level->gridHeight; i++) {
		for(int j = 0; j < g_level->gridWidth; j++) {
			//branch if target is a wall
			if(g_level->grid[i][j] == WALL) {
				//branch if there can be a corner top left of the target
				if(i + 1 < g_level->gridHeight && g_level->grid[i + 1][j] != WALL && j - 1 >= 0 && g_level->grid[i + 1][j - 1] == WALL) {
					cornerSize++;
					arr.insert(arr.end(), {
						//top left coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y],
						colorBlue.x, colorBlue.y, colorBlue.z,
						//bottom right coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yResize,
						colorBlue.x, colorBlue.y, colorBlue.z,
						//top right coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X], g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y],
						colorBlue.x, colorBlue.y, colorBlue.z
					});
				}
				//branch if there can be a corner bottom left of the target
				if(i - 1 >= 0 && g_level->grid[i - 1][j] != WALL && j - 1 >= 0 && g_level->grid[i - 1][j - 1] == WALL) {
					cornerSize++;
					arr.insert(arr.end(), {
						//top left coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y],
						colorBlue.x, colorBlue.y, colorBlue.z,
						//bottom right coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize,
						colorBlue.x, colorBlue.y, colorBlue.z,
						//top right coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y],
						colorBlue.x, colorBlue.y, colorBlue.z
					});
				}
				//branch if there can be a corner bottom right of the target
				if(i - 1 >= 0 && g_level->grid[i - 1][j] != WALL && j + 1 < g_level->gridWidth && g_level->grid[i - 1][j + 1] == WALL) {
					cornerSize++;
					arr.insert(arr.end(), {
						//top left coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y],
						colorBlue.x, colorBlue.y, colorBlue.z,
						//bottom left coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yResize,
						colorBlue.x, colorBlue.y, colorBlue.z,
						//top right coordinate
						g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y],
						colorBlue.x, colorBlue.y, colorBlue.z
					});
				}
				//branch if there can be a corner top right of the target
				if(i + 1 < g_level->gridHeight && g_level->grid[i + 1][j] != WALL && j + 1 < g_level->gridWidth && g_level->grid[i + 1][j + 1] == WALL) {
					cornerSize++;
					arr.insert(arr.end(), {
						//top left coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y],
						colorBlue.x, colorBlue.y, colorBlue.z,
						//bottom left coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize,
						colorBlue.x, colorBlue.y, colorBlue.z,
						//top right coordinate
						g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y],
						colorBlue.x, colorBlue.y, colorBlue.z
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
 * @brief Generate buffer array (x, y, z, xTex, yTex) * 4.
 * 
 * @param target 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> Maze::genWallCoordinates3D() {
	float Z = g_level->gridElementHeight / 2.f;
	//buffer array
	std::vector<GLfloat> arr;
	//fills in array with coordinates
	for(int i = 0; i < g_level->gridHeight; i++) {
		for(int j = 0; j < g_level->gridWidth; j++) {
			//branch if target isn't a wall or void
			if(g_level->grid[i][j] != WALL && g_level->grid[i][j] != VOID) {
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
