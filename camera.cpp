/* library */
#include "header/camera.h"
#include "header/dictionary.h"
#include "header/levelData.h"
#include <glm/gtc/matrix_transform.hpp>

extern LevelData *g_level;

Camera::~Camera() {}

Camera::Camera(const int width, const int height) {
	//set projection matrix
	projectionMatrix = glm::perspective(glm::radians(60.f), static_cast<float>(width) / height, 0.01f, 100.f);
    //set view matrix values
	camPos = glm::vec3(0.f, 0.f, 0.f);
	camFront = glm::vec3(0.f, 0.f, -1.f);
	camUp = glm::vec3(0.f, 1.f, 0.f);
	//set initial direction
	changeDirection(UP);
	//set view matrix
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera::changePosition(const float x, const float y) {
	//set z value
	float z = 0.02f;
	if(g_level->gamemode == THIRD_PERSON) z = 0.6f;
	//update view matrix position value
	camPos = glm::vec3(x + xOffset, y + yOffset, z);
	//update view matrix
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera::changeDirection(const int pacmanDirection) {
	//set direction value
	const int direction = pacmanDirection;
	//reset offset values
	xOffset = 0.f;
	yOffset = 0.f;
	if(g_level->gamemode == THIRD_PERSON) {
		//rotate camera
		yaw = 90.f;
		pitch = 150.f;
		//default world orientation
		camUp = glm::vec3(0.f, 1.f, 0.f);
		//set offset
		yOffset = -g_level->gridElementWidth * 6.f;
	} else {
		switch (direction) {
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
    camFront = glm::normalize(cameraDirection);
}
