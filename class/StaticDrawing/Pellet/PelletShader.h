#ifndef __PELLETSHADER_H_
#define __PELLETSHADER_H_
/* library */
#include <string>
//vertex shader
static const std::string pelletVertexShader = R"(
#version 430 core
//input
layout(location = 0) in vec3 gridPos;
layout(location = 1) in vec3 in_color;
//output
out vec3 vs_color;
//uniform
uniform mat4 u_collectionMatrix = mat4(1.f);

void main() {
    vs_color = in_color;
	gl_Position = u_collectionMatrix * vec4(gridPos, 1.f);
}
)";
//fragment shader
static const std::string pelletFragmentShader = R"(
#version 430 core
//input
in vec3 vs_color;
//output
out vec4 color;

void main() {
    color = vec4(vs_color, 1.f);
}
)";

#endif
