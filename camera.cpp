/* library */
#include "header/Camera.h"
#include "header/dictionary.h"
#include "header/levelData.h"
#include <iostream>

extern LevelData *g_level;

Camera::~Camera() {}

Camera::Camera(const int width, const int height) {
	//setup projection matrix
	fov = 60.f;
	nearPlane = 0.01f;
	farPlane = 100.f;
	projectionMatrix = glm::mat4(1.f);
	projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(width) / height, nearPlane, farPlane);
    //setup view matrix
	camPos = glm::vec3(0.f, 0.f, 0.f);
	camFront = glm::vec3(0.f, 0.f, -1.f);
	camUp = glm::vec3(0.f, 1.f, 0.f);
	//set initial direction
	changeDirection(UP);

	viewMatrix = glm::mat4(1.f);
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);

	firstMouse = true;
}

void Camera::changePosition(const float x, const float y) {
	float z = 0.01f;
	if(g_level->gamemode == THIRD_PERSON) z = 0.6f;
	camPos = glm::vec3(x + xOffset, y + yOffset, z);

	viewMatrix = glm::mat4(1.f);
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera::changeDirection(const int pacmanDirection) {
	direction = pacmanDirection;
	//reset offset values
	xOffset = 0.f;
	yOffset = 0.f;
	if(g_level->gamemode == THIRD_PERSON) {
		yaw = 90.f;
		pitch = 150.f;
		//default world orientation
		camUp = glm::vec3(0.f, 1.f, 0.f);
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

void Camera::updateViewMatrix(GLFWwindow* window, const float deltaTime, const int pacmanDirection) {
    /*camSpeed = 0.01f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camPos += camSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camPos -= camSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;

    viewMatrix = glm::mat4(1.f);
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);*/
}

void Camera::updateDirection(double xpos, double ypos) {
	/*if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw   += xoffset;
	pitch += yoffset;

	if(pitch > 89.0f) pitch =  89.0f;
	if(pitch < -89.0f) pitch = -89.0f;

	//restrict y movement (0 = first-person, -89 = third-person)
	//pitch = -90.f;
	//yaw = -90.f;

	glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(direction);*/
}
