/* library */
#include "Character.h"
#include "CharacterShader.h"
#include "../../../header/dictionary.h"
#include "../../../header/levelData.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
/* global data */
extern LevelData *g_level;
/**
 * @brief Destroy the Character:: Character object
 * 
 */
Character::~Character() {}

Character::Character() {
    //compile character shader
    shaderProgram = compileShader(characterVertexShader, characterFragmentShader);
}
/**
 * @brief Draw object by installing the shader program and binding the VAO and texture to the current rendering state
 * 
 */
void Character::draw() {
    auto samplerSlotLocation = glGetUniformLocation(shaderProgram, "u_texture");
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glUniform1i(samplerSlotLocation, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}
/**
 * @brief Generate array of grid positions and texture coordinates
 * 
 * @param row 
 * @param col 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> Character::genCoordinates(const int row, const int col) {
    GLfloat texPos = 0.f;
    std::vector<GLfloat> arr = {
        //top left grid and texture coordinate
        g_level->gridElement[std::make_pair(col, row)][TOP_LEFT][X], g_level->gridElement[std::make_pair(col, row)][TOP_LEFT][Y],	
        texPos, texPos + 0.25f,
        //bottom left grid and texture coordinate
        g_level->gridElement[std::make_pair(col, row)][BOTTOM_LEFT][X], g_level->gridElement[std::make_pair(col, row)][BOTTOM_LEFT][Y], 
        texPos, texPos,
        //bottom right rid and texture coordinate
        g_level->gridElement[std::make_pair(col, row)][BOTTOM_RIGHT][X], g_level->gridElement[std::make_pair(col, row)][BOTTOM_RIGHT][Y], 
        texPos + 0.16f, texPos,
        //top right grid and texture coordinate
        g_level->gridElement[std::make_pair(col, row)][TOP_RIGHT][X], g_level->gridElement[std::make_pair(col, row)][TOP_RIGHT][Y],	
        texPos + 0.16f, texPos + 0.25f
    };
    return arr;
}
/**
 * @brief Translate the character position on the x- and y-axis
 * 
 * @param x 
 * @param y 
 * @param shader 
 */
void Character::translatePos(const float xPos, const float yPos) {
    //Generate matrix to translate
    glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(xPos, yPos, 0.f));
    //get uniform to transform
    GLuint uniform = glGetUniformLocation(shaderProgram, "u_transformationPos");
    //send data from matrix to the uniform
    glUniformMatrix4fv(uniform, 1, false, glm::value_ptr(translation));
}
/**
 * @brief Translate the texture on the x- and y-axis
 * 
 * @param xPos 
 * @param yPos
 */
void Character::translateTex(const float xPos, const float yPos) {
	//Generate matrix to translate
	glm::mat3 translation = glm::translate(glm::mat3(1), glm::vec2(xPos, yPos));
    //get uniform to transform
	GLuint uniform = glGetUniformLocation(shaderProgram, "u_transformationTex");
	//Send data from matrix to the uniform
	glUniformMatrix3fv(uniform, 1, false, glm::value_ptr(translation));
}
/**
 * @brief Move character up the grid if possible and update position
 * 
 * @param row 
 * @param col 
 * @return true if character has reached the next location
 * @return false if character is still moving
 */
bool Character::movUp(int &row, int &col) {
    //branch if character isn't about to teleport
	if(col + 1 < g_level->gridHeight) {
        //branch if next location wont be a wall
        if(g_level->grid[col + 1][row] != WALL) {
            counter++;
            //translate position
            translatePos(xPos, (yPos += g_level->gridElementHeight / (float)(speed)));
            //branch if character has reached the next location
            if(counter == speed) return true;
        }
    } else {
        //update position
        col = 0;
        //teleport to the opposite side of the grid
        translatePos(xPos, yPos -= (float)(g_level->gridHeight - 1) * g_level->gridElementHeight);
    }
    return false;
}
/**
 * @brief Move character to the left of the grid if possible and update position
 * 
 * @param row 
 * @param col 
 * @return true if character has reached the next location
 * @return false if character is still moving
 */
bool Character::movLeft(int &row, int &col) {
    //branch if character isn't about to teleport
	if(row - 1 >= 0) {
        //branch if next location wont be a wall
        if(g_level->grid[col][row - 1] != WALL) {
            counter++;
            //translate position
            translatePos((xPos -= g_level->gridElementWidth / (float)(speed)), yPos);
            //branch if character has reached the next location
            if(counter == speed) return true;
        }
    } else {
        //update position
        row = g_level->gridWidth - 1;
        //teleport to the opposite side of the grid
        translatePos(xPos += (float)(g_level->gridWidth - 1) * g_level->gridElementWidth, yPos);
    }
    return false;
}
/**
 * @brief Move character down the grid if possible and update position
 * 
 * @param row 
 * @param col 
 * @return true if character has reached the next location
 * @return false if character is still moving
 */
bool Character::movDown(int &row, int &col) {
    //branch if character isn't about to teleport
	if(col - 1 >= 0) {
        //branch if next location wont be a wall
        if(g_level->grid[col - 1][row] != WALL) {
            counter++;
            //translate position
            translatePos(xPos, (yPos -= g_level->gridElementHeight / (float)(speed)));
            //branch if character has reached the next location
            if(counter == speed) return true;
        }
    } else {
        //update position
        col = g_level->gridHeight - 1;
        //teleport to the opposite side of the grid
        translatePos(xPos, yPos += (float)(g_level->gridHeight - 1) * g_level->gridElementHeight);
    }
    return false;
}
/**
 * @brief Move character to the right of the grid if possible and update position
 * 
 * @param row 
 * @param col 
 * @return true if character has reached the next location
 * @return false if character is still moving
 */
bool Character::movRight(int &row, int &col) {
    //branch if character isn't about to teleport
	if(row + 1 < g_level->gridWidth) {
        //branch if next location wont be a wall
        if(g_level->grid[col][row + 1] != WALL) {
            counter++;
            //translate position
            translatePos((xPos += g_level->gridElementWidth / (float)(speed)), yPos);
            //branch if character has reached the next location
            if(counter == speed) return true;
        }
	} else {
        //update position
        row = 0;
        //teleport to the opposite side of the grid
        translatePos(xPos -= (float)(g_level->gridWidth - 1) * g_level->gridElementWidth, yPos);
    }
    return false;
}
