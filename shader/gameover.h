#ifndef __GAMEOVERSHADER_H
#define __GAMEOVERSHADER_H
/* library */
#include <string>
//vertex shader
static const std::string gameoverVertexShader = R"(
#version 430 core
//input
layout(location = 0) in vec2 gridPos;
layout(location = 1) in vec2 texPos;
//output
out vec2 vs_texPos;

void main() {
	vs_texPos = texPos;
	gl_Position = vec4(gridPos, 0.0f, 1.0f);
}
)";
//fragment shader
static const std::string gameoverFragmentShader = R"(
#version 430 core
//input
in vec2 vs_texPos;
//output
out vec4 color;
//uniform
uniform sampler2D u_texture;

void main() {
	vec4 textColorA = texture(u_texture, vs_texPos);
	color = textColorA * vec4(1.f, 1.f, 1.f, 1.f);
}
)";


#endif
