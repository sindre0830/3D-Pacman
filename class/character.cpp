/* library */
#include "header/character.h"
#include "shader/character.h"
#include "../header/dictionary.h"
#include "../header/level.h"
#include "../header/camera.h"
#include "../header/functionality.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
/* global data */
extern Level *g_level;
extern Camera *g_camera;
/**
 * @brief Destroy the Character:: Character object
 * 
 */
Character::~Character() {
	glDeleteProgram(shaderProgram2D);
    glDeleteProgram(shaderProgram3D);
    destroyVAO(spriteVAO);
    destroyVAO(modelVAO);
}

Character::Character() {
    //compile character shader
    shaderProgram2D = compileShader(characterVertexShader, characterFragmentShader);
    shaderProgram3D = compileShader(characterModelVertexShader, characterModelFragmentShader);
}
/**
 * @brief Draw object by installing the shader program and binding the VAO and texture to the current rendering state
 * 
 */
void Character::draw() {
    glUseProgram(shaderProgram2D);
    glBindVertexArray(spriteVAO);
    if(g_level->gamemode == TWO_DIMENSIONAL) {
        //draw in 2D space
        glUniform1i(glGetUniformLocation(shaderProgram2D, "u_texture"), CHARACTER_TEXTURE);
        modelMatrix = glm::translate(glm::mat4(1.f), translation);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram2D, "u_transformationPos"), 1, false, glm::value_ptr(modelMatrix));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)(0));
    } else {
        //draw minimap
        glUniform1i(glGetUniformLocation(shaderProgram2D, "u_texture"), CHARACTER_TEXTURE);
        modelMatrix = getMinimapModelMatrix();
        modelMatrix = glm::translate(modelMatrix, translation);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram2D, "u_transformationPos"), 1, false, glm::value_ptr(modelMatrix));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)(0));
        //draw in 3D space
        glUseProgram(shaderProgram3D);
        glBindVertexArray(modelVAO);
        //get direction angel
        if(direction == UP) {
            modelDirection = 90.f;
        } else if(direction == LEFT) {
            modelDirection = 180.f;
        } else if(direction == DOWN) {
            modelDirection = 270.f;
        } else if(direction == RIGHT) {
            modelDirection = 0.f;
        }
        modelMatrix = glm::mat4(1.f);
        //set the initial position
        modelMatrix = glm::translate(modelMatrix, initialTranslation);
        //move model
        modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, 0.f));
        //rotate the model to stand up
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        //rotate the character to face the correct direction
        modelMatrix = glm::rotate(modelMatrix, glm::radians(modelDirection), glm::vec3(0.f, 1.f, 0.f));
        //scale down the model
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram3D, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));
        glDrawArrays(GL_TRIANGLES, 6, modelSize);
    }
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
    translation = glm::vec3(xPos, yPos, 0.f);
}
/**
 * @brief Translate the texture on the x- and y-axis
 * 
 * @param xPos 
 * @param yPos
 */
void Character::translateTex(const float xPos, const float yPos) {
    glUseProgram(shaderProgram2D);
	//Generate matrix to translate
	glm::mat3 translation = glm::translate(glm::mat3(1), glm::vec2(xPos, yPos));
	//Send data from matrix to the uniform
	glUniformMatrix3fv(glGetUniformLocation(shaderProgram2D, "u_transformationTex"), 1, false, glm::value_ptr(translation));
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
	if(col + 1 < g_level->gridHeight && g_level->grid[col + 1][row] != VOID) {
        //branch if next location wont be a wall
        if(g_level->grid[col + 1][row] != WALL && g_level->grid[col + 1][row] != VOID) {
            counter++;
            //translate position
            translatePos(xPos, (yPos += g_level->gridElementHeight / (float)(speed)));
            //branch if character has reached the next location
            if(counter == speed) return true;
        }
    } else {
        //update position
        col = g_level->colOffset / 2;
        //teleport to the opposite side of the grid
        translatePos(xPos, yPos -= (float)(g_level->gridHeight - g_level->colOffset - 1) * g_level->gridElementHeight);
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
	if(row - 1 >= 0 && g_level->grid[col][row - 1] != VOID) {
        //branch if next location wont be a wall
        if(g_level->grid[col][row - 1] != WALL && g_level->grid[col][row - 1] != VOID) {
            counter++;
            //translate position
            translatePos((xPos -= g_level->gridElementWidth / (float)(speed)), yPos);
            //branch if character has reached the next location
            if(counter == speed) return true;
        }
    } else {
        //update position
        row = g_level->gridWidth - (g_level->rowOffset / 2) - 1;
        //teleport to the opposite side of the grid
        translatePos(xPos += (float)(g_level->gridWidth - g_level->rowOffset - 1) * g_level->gridElementWidth, yPos);
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
	if(col - 1 >= 0 && g_level->grid[col - 1][row] != VOID) {
        //branch if next location wont be a wall
        if(g_level->grid[col - 1][row] != WALL && g_level->grid[col - 1][row] != VOID) {
            counter++;
            //translate position
            translatePos(xPos, (yPos -= g_level->gridElementHeight / (float)(speed)));
            //branch if character has reached the next location
            if(counter == speed) return true;
        }
    } else {
        //update position
        col = g_level->gridHeight - (g_level->colOffset / 2) - 1;
        //teleport to the opposite side of the grid
        translatePos(xPos, yPos += (float)(g_level->gridHeight - g_level->colOffset - 1) * g_level->gridElementHeight);
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
	if(row + 1 < g_level->gridWidth && g_level->grid[col][row + 1] != VOID) {
        //branch if next location wont be a wall
        if(g_level->grid[col][row + 1] != WALL && g_level->grid[col][row + 1] != VOID) {
            counter++;
            //translate position
            translatePos((xPos += g_level->gridElementWidth / (float)(speed)), yPos);
            //branch if character has reached the next location
            if(counter == speed) return true;
        }
	} else {
        //update position
        row = g_level->rowOffset / 2;
        //teleport to the opposite side of the grid
        translatePos(xPos -= (float)(g_level->gridWidth - g_level->rowOffset - 1) * g_level->gridElementWidth, yPos);
    }
    return false;
}
