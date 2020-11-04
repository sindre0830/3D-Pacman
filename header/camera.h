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
        float yaw, pitch, lastX, lastY, camSpeed;
        bool firstMouse;
        glm::vec3 camFront, camPos, camUp;
        const float sensitivity = 0.05f;
    public:
        /* public data */
        glm::mat4 viewMatrix;
        /* public functionality */
        ~Camera();
        Camera();
        void updateViewMatrix(GLFWwindow *window, const float deltaTime);
        void updateDirection(double xpos, double ypos);
        void processInput(GLFWwindow *window, float deltaTime, glm::vec3 &camPos, glm::vec3 camFront, float &camSpeed, glm::vec3 camUp);

};
#endif
