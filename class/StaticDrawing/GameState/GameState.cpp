/* library */
#include "GameState.h"
#include "GameStateShader.h"
#include "../../../header/levelData.h"
/* global data */
extern LevelData *g_level;
/**
 * @brief Destroy the Gameover:: Gameover object
 * 
 */
GameState::~GameState() {}
/**
 * @brief Construct a new Gameover:: Gameover object
 * 
 */
GameState::GameState() {
    //compile gameover shader
    shaderProgram = compileShader(gameoverVertexShader, gameoverFragmentShader);
    //create VAO
	std::vector<GLfloat> arr = genCoordinates();
    VAO = genObject(arr, 1);
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
void GameState::draw() {
    auto samplerSlotLocation = glGetUniformLocation(shaderProgram, "u_texture");
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glUniform1i(samplerSlotLocation, 2);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}
/**
 * @brief Generate array of grid positions and texture coordinates
 * 
 * @return std::vector<GLfloat> 
 */
std::vector<GLfloat> GameState::genCoordinates() {
    std::vector<GLfloat> arr = {
        //top left grid and texture coordinate
        -0.5f, 0.5f,
        0.f, 1.f,
        //bottom left grid and texture coordinate
        -0.5f, -0.5f,
        0.f, 0.f,
        //bottom right grid and texture coordinate
        0.5f, -0.5f,
        1.f, 0.f,
        //top right grid and texture coordinate
        0.5f, 0.5f,
        1.f, 1.f
    };
    return arr;
}
