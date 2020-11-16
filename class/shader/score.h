#ifndef __SCORESHADER_H_
#define __SCORESHADER_H_
/* library */
#include <string>
//vertex shader
static const std::string numberVertexShader = R"(
	#version 430 core
    /* input */
	layout(location = 0) in vec2 a_gridPos;
	layout(location = 1) in vec2 a_texPos;
    /* output */
	out vec2 vs_texPos;
    /* uniform */
	uniform mat3 u_transformationTex = mat3(1);
    /**
     * Main vertex shader program.
     */
	void main() {
		//transform texture coordinates
		vec3 newTexPos = u_transformationTex * vec3(a_texPos, 1.f);
		vs_texPos = vec2(newTexPos);
		gl_Position = vec4(a_gridPos, 0.f, 1.f);
	}
)";
//fragment shader
static const std::string numberFragmentShader = R"(
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
