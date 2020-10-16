#ifndef __MAZESHADER_H_
#define __MAZESHADER_H_
/* library */
#include <string>
//vertex shader
static const std::string wallVertexShader = R"(
#version 430 core
//input
layout(location = 0) in vec2 a_Position;

void main() {
	gl_Position = vec4(a_Position, 0.f, 1.f);
}
)";
//fragment shader
static const std::string wallFragmentShader = R"(
#version 430 core
//output
out vec4 color;

void main() {
    //set color blue
    color = vec4(0.09f, 0.09f, 0.4f, 1.f);
}
)";

#endif
