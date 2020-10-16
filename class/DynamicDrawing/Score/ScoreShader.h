#ifndef __SCORESHADER_H_
#define __SCORESHADER_H_
/* library */
#include <string>
//vertex shader
static const std::string numberVertexShader = R"(
#version 430 core
//input
layout(location = 0) in vec2 gridPos;
layout(location = 1) in vec2 texPos;
//output
out vec2 vs_texPos;
//uniform
uniform mat3 u_transformationTex = mat3(1);

void main() {
	//transform texture coordinates
	vec3 newTexPos = u_transformationTex * vec3(texPos, 1.0f);
	vs_texPos = vec2(newTexPos);

	gl_Position = vec4(gridPos, 0.0f, 1.0f);
}
)";
//fragment shader
static const std::string numberFragmentShader = R"(
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
