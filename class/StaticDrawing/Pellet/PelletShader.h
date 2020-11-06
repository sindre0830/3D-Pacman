#ifndef __PELLETSHADER_H_
#define __PELLETSHADER_H_
/* library */
#include <string>
//vertex shader
static const std::string pelletVertexShader = R"(
    #version 430 core
    //input
    layout(location = 0) in vec3 gridPos;
    layout(location = 1) in vec3 normals;
    //uniform
    layout(location=0) uniform mat4 u_modelMatrix = mat4(1);
    layout(location=1) uniform mat4 u_viewMatrix = mat4(1);
    layout(location=2) uniform mat4 u_projectionMatrix = mat4(1);

    void main() {
        mat3 normalMatrix = transpose(inverse(mat3(u_viewMatrix * u_modelMatrix)));

        gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(gridPos, 1.f);
    }
)";
//fragment shader
static const std::string pelletFragmentShader = R"(
    #version 430 core
    //output
    out vec4 color;

    void main() {
        color = vec4(1.f, 1.f, 1.f, 1.f);
    }
)";

#endif
