/* library */
#include "Character.h"
#include "CharacterShader.h"
#include "../../../header/dictionary.h"
#include "../../../header/levelData.h"
#include "../../../header/Camera.h"
#include "../../../header/function.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
/* global data */
extern LevelData *g_level;
extern Camera *g_camera;
/**
 * @brief Destroy the Character:: Character object
 * 
 */
Character::~Character() {
    glDeleteProgram(modelShaderProgram);
    destroyVAO(modelVAO);
}

Character::Character() {
    //compile character shader
    shaderProgram = compileShader(characterVertexShader, characterFragmentShader);
    modelShaderProgram = compileModelShader(characterModelVertexShader, characterModelFragmentShader);
}
/**
 * @brief Draw object by installing the shader program and binding the VAO and texture to the current rendering state
 * 
 */
void Character::draw() {
    if(g_level->displayMinimap) {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //
        glUniform1i(glGetUniformLocation(shaderProgram, "u_texture"), 0);
        //
        modelMatrix = getMinimapModelMatrix();
        modelMatrix = glm::translate(modelMatrix, translation);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_transformationPos"), 1, false, glm::value_ptr(modelMatrix));
        //
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
    }
    if(g_level->gamemode == TWO_DIMENSIONAL) {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glUniform1i(glGetUniformLocation(shaderProgram, "u_texture"), 0);
        modelMatrix = glm::translate(glm::mat4(1.f), translation);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_transformationPos"), 1, false, glm::value_ptr(modelMatrix));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
    } else {
        /*if(direction == UP) {
            modelDirection == -45.f;
        } else if(direction == LEFT) {
            modelDirection == 0.f;
        } else if(direction == DOWN) {
            modelDirection == 45.f;
        } else if(direction == RIGHT) {
            modelDirection == 90.f;
        }*/
        float size = 0.02f;
        //if(isPacman) size = 0.02f;

        glUseProgram(modelShaderProgram);
        glBindVertexArray(modelVAO);
        modelMatrix = glm::mat4(1.f);
        //set the initial position
        modelMatrix = glm::translate(modelMatrix, initialTranslation);
        //move model
        modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, 0.f));
        //rotate the model to stand up
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        //rotate the character to face the correct direction //doesn't work
        //modelMatrix = glm::rotate(modelMatrix, glm::radians(modelDirection), glm::vec3(0.f, 0.f, 1.f));
        //scale down the model
        modelMatrix = glm::scale(modelMatrix, glm::vec3(size));
        glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(modelShaderProgram, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(g_camera->projectionMatrix));

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
    /*if(g_level->displayMinimap) {
        modelMatrix = glm::mat4(1.f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.75f, 0.75f, 0.f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, 0.f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_transformationPos"), 1, false, glm::value_ptr(modelMatrix));
        draw(glm::mat4(1.f));
    }
    modelMatrix = glm::mat4(1.f);*/
    //Generate matrix to translate
    /*modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, 0.f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_transformationPos"), 1, false, glm::value_ptr(modelMatrix));*/
    //send data from matrix to the uniform
    translation = glm::vec3(xPos, yPos, 0.f);
}
/**
 * @brief Translate the texture on the x- and y-axis
 * 
 * @param xPos 
 * @param yPos
 */
void Character::translateTex(const float xPos, const float yPos) {
    glUseProgram(shaderProgram);
	//Generate matrix to translate
	glm::mat3 translation = glm::translate(glm::mat3(1), glm::vec2(xPos, yPos));
	//Send data from matrix to the uniform
	glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "u_transformationTex"), 1, false, glm::value_ptr(translation));
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
