/* library */
#include "header/score.h"
#include "shader/score.h"
#include "dictionary.h"
#include "level.h"
#include "functionality.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
/* global data */
extern Level *g_level;
/**
 * @brief Destroy the Score:: Score object.
 * 
 */
Score::~Score() {
	glDeleteProgram(shaderProgram);
    destroyVAO(VAO);
}
/**
 * @brief Construct a new Score:: Score object.
 * 
 * @param col 
 * @param row 
 */
Score::Score(const int col, const int row) {
    //compile scoreboard shader
    shaderProgram = compileShader(numberVertexShader, numberFragmentShader);
    //create VAO
	std::vector<GLfloat> arr = genCoordinates(col, row);
    VAO = genObject(arr, 1);
    //specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*)(2 * sizeof(GLfloat)));
}
/**
 * @brief Draw object by activating the shader program and binding the VAO and uniforms to the current rendering state.
 * 
 */
void Score::draw() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glUniform1i(glGetUniformLocation(shaderProgram, "u_texture"), NUMBER_TEXTURE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
    glUseProgram(0);
}
/**
 * @brief Generate array of grid positions and texture coordinates.
 * 
 * @param col 
 * @param row 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> Score::genCoordinates(const int col, const int row) {
    GLfloat texPos = 0.f;
    float
		//resize pellet
		xResize = (float)(g_level->gridElementWidth / 5.f),
		yResize = (float)(g_level->gridElementHeight / 5.f);
    std::vector<GLfloat> arr = {
        //top left grid and texture coordinate
        g_level->gridElement[std::make_pair(col, row)][TOP_LEFT][X] + xResize, g_level->gridElement[std::make_pair(col, row)][TOP_LEFT][Y] - yResize,	
        texPos, texPos + 1.f,
        //bottom left grid and texture coordinate
        g_level->gridElement[std::make_pair(col, row)][BOTTOM_LEFT][X] + xResize, g_level->gridElement[std::make_pair(col, row)][BOTTOM_LEFT][Y] + yResize, 
        texPos, texPos,
        //bottom right grid and texture coordinate
        g_level->gridElement[std::make_pair(col, row)][BOTTOM_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(col, row)][BOTTOM_RIGHT][Y] + yResize, 
        texPos + (1.f / 10.f), texPos,
        //top right grid and texture coordinate
        g_level->gridElement[std::make_pair(col, row)][TOP_RIGHT][X] - xResize, g_level->gridElement[std::make_pair(col, row)][TOP_RIGHT][Y] - yResize,	
        texPos + (1.f / 10.f), texPos + 1.f
    };
    return arr;
}
/**
 * @brief Update number if it is different.
 * 
 * @param num 
 */
void Score::update(const int num) {
    //branch if new number is different from the last number and update texture
    if(num != lastNumber) translateTex((float)(num) / 10.f);
    //set new number for next update
    lastNumber = num;
}
/**
 * @brief Translate the texture on the x-axis.
 * 
 * @param xPos 
 */
void Score::translateTex(const float xPos) {
    glUseProgram(shaderProgram);
    //Generate matrix to translate
	glm::mat3 translation = glm::translate(glm::mat3(1), glm::vec2(xPos, 0.f));
	//send data from matrix to the uniform
	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "u_transformationTex"), 1, false, glm::value_ptr(translation));
}
