#ifndef __GAMESTATESHADER_H
#define __GAMESTATESHADER_H
/* library */
#include <string>
//vertex shader
static const std::string gameoverVertexShader = R"(
	#version 430 core
    /* input */
	layout(location = 0) in vec2 a_gridPos;
	layout(location = 1) in vec2 a_texPos;
    /* output */
	out vec2 vs_texPos;
    /**
     * Main vertex shader program.
     */
	void main() {
		vs_texPos = a_texPos;
		gl_Position = vec4(a_gridPos, -1.f, 1.f);
	}
)";
//fragment shader
static const std::string gameoverFragmentShader = R"(
	#version 430 core
    /* input */
	in vec2 vs_texPos;
    /* output */
	out vec4 color;
    /* uniform */
	uniform sampler2D u_texture;
    /**
     * Main fragment shader program.
     */
	void main() {
		color = texture(u_texture, vs_texPos);
	}
)";
#endif
