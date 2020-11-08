/* library */
#include "Pellet.h"
#include "PelletShader.h"
#include "../../../header/dictionary.h"
#include "../../../header/levelData.h"

/* global data */
extern LevelData *g_level;
/**
 * @brief Destroy the Pellet:: Pellet object
 * 
 */
Pellet::~Pellet() {}
/**
 * @brief Construct a new Pellet:: Pellet object
 * 
 */
Pellet::Pellet() {
    //compile pellet shader
	shaderProgram = compileShader(pelletVertexShader, pelletFragmentShader);
    //create VAO
	std::vector<GLfloat> arr = genCoordinates();
    VAO = genObject(arr, g_level->pelletSize * 5);
	//set the vertex attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
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
}
/**
 * @brief Draw object by installing the shader program and binding the VAO to the current rendering state
 * 
 */
void Pellet::draw(glm::mat4 collectionMatrix) {
	if(g_level->gamemode == TWO_DIMENSIONAL) {
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
		glDrawElements(GL_TRIANGLES, (6 * g_level->pelletSize * 5), GL_UNSIGNED_INT, (const void*)0);
	} else {
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_collectionMatrix"), 1, GL_FALSE, glm::value_ptr(collectionMatrix));
		glDrawElements(GL_TRIANGLES, (6 * g_level->pelletSize * 5), GL_UNSIGNED_INT, (const void*)0);
	}
}
/**
 * @brief Hide pellet by modifying the buffer array
 * 
 * @param y 
 * @param x 
 */
void Pellet::hidePellet(const int col, const int row) {
	GLfloat z = 10.f;
	for(int i = 8, n = 0; n < (4 * 5); i += 24, n++) {
		glBufferSubData(GL_ARRAY_BUFFER, bufferPos[std::make_pair(col, row)] + i, sizeof(GLfloat), &z);
	}
}
/**
 * @brief Generate buffer array ((x, y, z, r, g, b) * 4) * 5
 * 
 * @param target 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> Pellet::genCoordinates() {
	float
		//display pellet
		z = (g_level->gridElementHeight / 2.8f) / 2.8f,
		//resize pellet
		xResize = g_level->gridElementWidth / 2.8f,
		yResize = g_level->gridElementHeight / 2.8f,
		//rotate pellet
		xRotate = g_level->gridElementWidth / 2.8f,
		yRotate = g_level->gridElementHeight / 2.8f;
	//buffer array
	std::vector<GLfloat> arr;
	//fills in array with coordinates
	for (int i = 0; i < g_level->gridHeight; i++) {
		for (int j = 0; j < g_level->gridWidth; j++) {
			//branch if target is pellet and insert data or if target is a magic pellet and make it twice as big
			if (g_level->grid[i][j] == PELLET) {
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, -z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize, -z,
					1.f, 1.f, 1.f
				});
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize, -z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, -z,
					1.f, 1.f, 1.f
				});
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize, -z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, -z,
					1.f, 1.f, 1.f
				});
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, -z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize, -z,
					1.f, 1.f, 1.f
				});
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, z,
					1.f, 1.f, 1.f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize, z,
					1.f, 1.f, 1.f
				});
				//original code for 2D VAO
				/*arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, z,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + yResize, z,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, z,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - yResize, z
				});*/
			} else if (g_level->grid[i][j] == MAGICPELLET) {
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - (yResize * 2.f), -z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - (yResize * 2.f), -z * 2.f,
					0.47f, 0.02f, 0.02f
				});
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + (yResize * 2.f), -z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + (yResize * 2.f), -z * 2.f,
					0.47f, 0.02f, 0.02f
				});
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + (yResize * 2.f), -z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - (yResize * 2.f), -z * 2.f,
					0.47f, 0.02f, 0.02f
				});
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + (yResize * 2.f), -z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - (yResize * 2.f), -z * 2.f,
					0.47f, 0.02f, 0.02f
				});
				arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - (xResize * 2.f), g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - (yResize * 2.f), z * 2.f,
					0.47f, 0.02f, 0.02f
				});
				//original code for 2D VAO
				/*arr.insert(arr.end(), {
					g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][X] + (xResize * 2.3f), g_level->gridElement[std::make_pair(i, j)][TOP_LEFT][Y] - yRotate, z,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][X] + xRotate, g_level->gridElement[std::make_pair(i, j)][BOTTOM_LEFT][Y] + (yResize * 2.3f), z,
					g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][X] - (xResize * 2.3f), g_level->gridElement[std::make_pair(i, j)][BOTTOM_RIGHT][Y] + yRotate, z,
					g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][X] - xRotate, g_level->gridElement[std::make_pair(i, j)][TOP_RIGHT][Y] - (yResize * 2.3f), z
				});*/
			}
		}
	}
	return arr;
}