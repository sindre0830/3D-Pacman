#ifndef __CAMERA_H
#define __CAMERA_H
/* library */
#include <glm/glm.hpp>
/**
 * @brief Leaf class
 * 
 */
class Camera {
    private:
        /* private data */
        float xOffset = 0, yOffset = 0;
        glm::vec3 camFront, camPos, camUp;
    public:
        /* public data */
        glm::mat4 viewMatrix, projectionMatrix;
        /* public functionality */
        ~Camera();
        Camera(const int width, const int height);
        void changePosition(const float x, const float y);
        void changeDirection(const int pacmanDirection);

};
#endif
