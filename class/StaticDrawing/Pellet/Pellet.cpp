/* library */
#include "Pellet.h"
#include "PelletShader.h"
#include "../../../header/dictionary.h"
#include "../../../header/function.h"
#include "../../../header/levelData.h"
#include "../../../header/Camera.h"
#include "../../../header/tiny_obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

/* global data */
extern LevelData *g_level;
extern Camera *g_camera;
/**
 * @brief Destroy the Pellet:: Pellet object
 * 
 */
Pellet::~Pellet() {
	destroyVAO(modelVAO);
}
/**
 * @brief Construct a new Pellet:: Pellet object
 * 
 */
Pellet::Pellet(glm::mat4 modelMatrix, glm::mat4 projectionMatrix) {
    //compile pellet shader
	shaderProgram = compileShader(pelletVertexShader, pelletFragmentShader);
    //create VAO
	std::vector<GLfloat> arr = genCoordinates();
    VAO = genObject(arr, g_level->pelletSize);
	//set the vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void*)0);
	//set buffer position in refrence to position in level
	for(int i = 0, n = 0; i < g_level->gridHeight; i++) {
		for(int j = 0; j < g_level->gridWidth; j++) {
			//branch if target is either a pellet or a magic pellet
			if(g_level->grid[i][j] == PELLET || g_level->grid[i][j] == MAGICPELLET) {
				bufferPos[std::make_pair(i, j)] = n * pelletByteSize;
				n++;
			}
		}
	}
	/* Pellet Model Construction */
	modelSize = 0;
	modelVAO = LoadModel("models/pellet/", "pellet.obj", modelSize);
	//send in initial uniform data
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}
/**
 * @brief Draw object by installing the shader program and binding the VAO to the current rendering state
 * 
 */
void Pellet::draw(glm::mat4 modelMatrix, glm::mat4 projectionMatrix) {
	glUseProgram(shaderProgram);
	if(g_level->gamemode == TWO_DIMENSIONAL) {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, (6 * g_level->pelletSize), GL_UNSIGNED_INT, (const void*)0);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
	} else {
		glBindVertexArray(modelVAO);
        glDrawArrays(GL_TRIANGLES, 6, modelSize);
		//send in initial uniform data
		glm::mat4 modelMatrixPellet(1.f);
		modelMatrixPellet = glm::translate(modelMatrixPellet, glm::vec3(0.f, -0.008f, 0.f));
		modelMatrixPellet = glm::scale(modelMatrixPellet, glm::vec3(0.008f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrixPellet));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}
}
/**
 * @brief Hide pellet by modifying the buffer array
 * 
 * @param y 
 * @param x 
 */
void Pellet::hidePellet(const int col, const int row) {
	GLfloat Z = 1.f;
	for(int i = 8; i < pelletByteSize; i += 12) {
		//glBufferSubData(GL_ARRAY_BUFFER, bufferPos[std::make_pair(col, row)] + i, sizeof(GLfloat), &Z);
	}
}
/**
 * @brief Generate buffer array (x, y, display) * 4
 * 
 * @param target 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> Pellet::genCoordinates() {
	float
		//display pellet
		Z = -0.4f,
		//resize pellet
		xResize = g_level->gridElementWidth / 2.8f,
		yResize = g_level->gridElementHeight / 2.8f,
		//rotate pellet
		xRotate = g_level->gridElementWidth / 2.f,
		yRotate = g_level->gridElementHeight / 2.f;
	//buffer array
	std::vector<GLfloat> arr;
	//fills in array with coordinates
	for (int i = 0; i < g_level->gridHeight; i++) {
		for (int j = 0; j < g_level->gridWidth; j++) {
			//branch if target is pellet and insert data or if target is a magic pellet and make it twice as big
			if (g_level->grid[i][j] == PELLET) {
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, Z,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize, Z,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, Z,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize, Z
				});
			} else if (g_level->grid[i][j] == MAGICPELLET) {
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + (xResize * 2.3f), g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, Z,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + (yResize * 2.3f), Z,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - (xResize * 2.3f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, Z,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - (yResize * 2.3f), Z
				});
			}
		}
	}
	return arr;
}
