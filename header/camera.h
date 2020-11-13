#ifndef __CAMERA_H
#define __CAMERA_H
/* library */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
/**
 * @brief Leaf class
 * 
 */
class Camera {
    private:
        /* private data */
        float fov, nearPlane, farPlane;
        float yaw, pitch, lastX, lastY, camSpeed, xOffset = 0, yOffset = 0;
        bool firstMouse;
        glm::vec3 camFront, camPos, camUp;
        const float sensitivity = 0.05f;
    public:
        /* public data */
        int direction = 0;
        glm::mat4 viewMatrix, projectionMatrix;
        /* public functionality */
        ~Camera();
        Camera(const int width, const int height);
        void changePosition(const float x, const float y);
        void changeDirection(const int pacmanDirection);
        void updateViewMatrix(GLFWwindow *window, const float deltaTime, const int pacmanDirection);
        void updateDirection(double xpos, double ypos);
        void processInput(GLFWwindow *window, float deltaTime, glm::vec3 &camPos, glm::vec3 camFront, float &camSpeed, glm::vec3 camUp);

};
#endif
