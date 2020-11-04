/* library */
#include "header/Camera.h"
#include "header/dictionary.h"
#include "header/levelData.h"

extern LevelData *g_level;

Camera::~Camera() {}

Camera::Camera() {
    //setup camera
	camPos = glm::vec3(0.f, 0.f, 3.f);
	camFront = glm::vec3(0.f, 0.f, -1.f);
	camUp = glm::vec3(0.f, 1.f, 0.f);

	viewMatrix = glm::mat4(1.f);
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);

	camSpeed = 0.05f;

	yaw = -90.0f,
	pitch = 0.f;

	firstMouse = true;
}

void Camera::updateViewMatrix(GLFWwindow* window, const float deltaTime) {
    camSpeed = 0.01f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camPos += camSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camPos -= camSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;

    viewMatrix = glm::mat4(1.f);
	viewMatrix = glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera::updateDirection(double xpos, double ypos) {
	if (firstMouse) {
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
	pitch = 0.f;

	glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(direction);
}
