#ifndef __PELLETSHADER_H_
#define __PELLETSHADER_H_
/* library */
#include <string>
//vertex shader
static const std::string pelletVertexShader = R"(
#version 430 core
//input
layout(location = 0) in vec3 gridPos;

void main() {
	gl_Position = vec4(gridPos, 1.f);
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
