/* library */
#include "camera.h"
#include "dictionary.h"
#include "level.h"
#include <glm/gtc/matrix_transform.hpp>
/* global data */
extern Level *g_level;
/**
 * @brief Destroy the Camera:: Camera object.
 * 
 */
Camera::~Camera() {}
/**
 * @brief Construct a new Camera:: Camera object.
 * 
 * @param width 
 * @param height 
 */
Camera::Camera(const int width, const int height) {
	//set projection matrix
	projectionMatrix = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.01f, 100.f);
    //set initial view matrix values
	camPos = glm::vec3(0.f, 0.f, 0.f);
	camFront = glm::vec3(0.f, 0.f, -1.f);
	camUp = glm::vec3(0.f, 1.f, 0.f);
	//set initial direction
	changeDirection(UP);
	//set view matrix
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
}
/**
 * @brief Change camera position according to the main character.
 * 
 * @param x 
 * @param y 
 */
void Camera::changePosition(const float x, const float y) {
	//set z value
	float z = 0.02f;
	//branch if gamemode is third person and set the z value to be higher
	if(g_level->gamemode == THIRD_PERSON) z = 0.6f;
	//update view matrix position value
	camPos = glm::vec3(x + xOffset, y + yOffset, z);
	//update view matrix
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
}
/**
 * @brief Change direction accroding to the main character.
 * 
 * @param pacmanDirection 
 */
void Camera::changeDirection(const int pacmanDirection) {
	//set direction value
	const int direction = pacmanDirection;
	//reset offset values
	xOffset = 0.f;
	yOffset = 0.f;
	//declare yaw and pitch
	float yaw, pitch;
	//branch if gamemode is third person and update view matrix to show a tilted top to down view
	if(g_level->gamemode == THIRD_PERSON) {
		//rotate camera
		yaw = 90.f;
		pitch = 150.f;
		//default world orientation
		camUp = glm::vec3(0.f, 1.f, 0.f);
		//set offset
		yOffset = -g_level->gridElementWidth * 6.f;
	//branch if gamemode is first person and set view matrix according to the direction of the main character
	} else {
		switch(direction) {
			case UP:
				//rotate camera
				yaw = 90.f;
				pitch = 90.f;
				//default world orientation
				camUp = glm::vec3(0.f, 1.f, 0.f);
				//set offset
				yOffset = -g_level->gridElementWidth / 1.5f;
				break;
			case DOWN:
				//rotate camera
				yaw = -90.f;
				pitch = -90.f;
				//default world orientation
				camUp = glm::vec3(0.f, 1.f, 0.f);
				//set offset
				yOffset = g_level->gridElementWidth / 1.5f;
				break;
			case LEFT:
				//rotate camera
				yaw = -180.f;
				pitch = 0.f;
				//turn world orientation
				camUp = glm::vec3(0.f, 0.f, 1.f);
				//set offset
				xOffset = g_level->gridElementWidth / 1.5f;
				break;
			case RIGHT:
				//rotate camera
				yaw = 0.f;
				pitch = 0.f;
				//turn world orientation
				camUp = glm::vec3(0.f, 0.f, 1.f);
				//set offset
				xOffset = -g_level->gridElementWidth / 1.5f;
				break;
		}
	}
	//update camera direction
	glm::vec3 cameraDirection;
    cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDirection.y = sin(glm::radians(pitch));
    cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//update view matrix front value
    camFront = glm::normalize(cameraDirection);
}
