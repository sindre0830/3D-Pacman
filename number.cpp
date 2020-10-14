/* library */
#include "header/number.h"
#include "shader/number.h"
#include "header/dictionary.h"
#include "header/levelData.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
/* global data */
extern LevelData *g_level;
/**
 * @brief Destroy the Scoreboard:: Scoreboard object
 * 
 */
Number::~Number() {}
/**
 * @brief Construct a new Scoreboard:: Scoreboard object
 * 
 * @param col 
 * @param row 
 */
Number::Number(const int col, const int row) {
    //compile scoreboard shader
    shapeShaderProgram = compileShader(numberVertexShader, numberFragmentShader);
    //create VAO
	std::vector<GLfloat> arr = genCoordinates(col, row);
    shapeVAO = genObject(arr, 1);
    //specify the layout of the vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}
/**
 * @brief Draw object by installing the shader program and binding the VAO and texture to the current rendering state
 * 
 */
void Number::draw() {
    GLint samplerSlotLocation = glGetUniformLocation(shapeShaderProgram, "u_texture");
    glUseProgram(shapeShaderProgram);
    glBindVertexArray(shapeVAO);
    glUniform1i(samplerSlotLocation, 1);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}
/**
 * @brief Generate array of grid positions and texture coordinates
 * 
 * @param col 
 * @param row 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> Number::genCoordinates(const int col, const int row) {
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
 * @brief Update number if it is different
 * 
 * @param num 
 */
void Number::update(const int num) {
    //branch if new number is different from the last number and update texture
    if(num != lastNumber) translateTex((float)(num) / 10.f);
    //set new number for next update
    lastNumber = num;
}
/**
 * @brief Translate the texture on the x-axis
 * 
 * @param xPos 
 */
void Number::translateTex(const float xPos) {
    //Generate matrix to translate
	glm::mat3 translation = glm::translate(glm::mat3(1), glm::vec2(xPos, 0.f));
    //get uniform to transform
	GLuint uniform = glGetUniformLocation(shapeShaderProgram, "u_transformationTex");
	//send data from matrix to the uniform
	glUniformMatrix3fv(uniform, 1, false, glm::value_ptr(translation));
}
