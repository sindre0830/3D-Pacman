#ifndef __CHARACTERSHADER_H_
#define __CHARACTERSHADER_H_
/* library */
#include <string>
//2D vertex shader
static const std::string characterVertexShader = R"(
	#version 430 core
    /* input */
	layout(location = 0) in vec2 a_gridPos;
	layout(location = 1) in vec2 a_texPos;
    /* output */
	out vec2 vs_texPos;
    /* uniform */
	uniform mat4 u_transformationPos = mat4(1);
	uniform mat3 u_transformationTex = mat3(1);
    /**
     * Main vertex shader program.
     */
	void main() {
		//transform texture coordinates
		vec3 newTexPos = u_transformationTex * vec3(a_texPos, 1.0f);
		vs_texPos = vec2(newTexPos);
		//transform grid coordinates
		gl_Position = u_transformationPos * vec4(a_gridPos, -0.5f, 1.0f);
	}
)";
//2D fragment shader
static const std::string characterFragmentShader = R"(
	#version 430 core
    /* input */
	in vec2 vs_texPos;
    /* output */
	out vec4 color;
    /* uniform */
	uniform sampler2D u_texture;
	uniform int u_changeColor = 0;
    /**
     * Main fragment shader program.
     */
	void main() {
		vec4 texColor = texture(u_texture, vs_texPos);
		//branch if color should change
		if(u_changeColor == 1) {
			color = texColor * vec4(0.5f, 1.f, 0.5f, 1.f);
		} else color = texColor * vec4(1.f, 1.f, 1.f, 1.f);
	}
)";
//3D vertex shader
static const std::string characterModelVertexShader = R"(
	#version 430 core
    /* input */
    layout(location = 0) in vec3 a_gridPos;
    layout(location = 1) in vec3 a_normal;
    /* output */
	out vec4 vs_fragPos;
	out vec3 vs_normal;
    /* uniform */
    uniform mat4 u_modelMatrix = mat4(1);
    uniform mat4 u_viewMatrix = mat4(1);
    uniform mat4 u_projectionMatrix = mat4(1);
    /**
     * Main vertex shader program.
     */
	void main() {
		//We need these in a different shader later down the pipeline, so we need to send them along. Can't just call in a_Position unfortunately.
		vs_fragPos = vec4(a_gridPos, 1.f);
		//Find the correct values for our normals given that we move our object around in the world and the normals change quite a bit.
		mat3 normalMatrix = transpose(inverse(mat3(u_viewMatrix * u_modelMatrix)));
		//Then normalize those new values so we do not accidentally go above length = 1. Also normalize the normals themselves beforehand, just to be sure calculations are accurate.
		vs_normal = normalize(normalMatrix * normalize(a_normal));
		
        gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_gridPos, 1.f);

	}
)";
//3D fragment shader
static const std::string characterModelFragmentShader = R"(
	#version 430 core
    /* input */
	in vec4 vs_fragPos;
	in vec3 vs_normal;
    /* output */
    out vec4 color;
    /* uniform */
    uniform mat4 u_modelMatrix = mat4(1);
    uniform mat4 u_viewMatrix = mat4(1);
	uniform vec3 u_objectColor = vec3(1.f, 0.f, 0.f);
	uniform vec3 u_lightColor = vec3(1.f, 1.f, 1.f);
	uniform vec3 u_lightPos = vec3(1.f, 0.f, 1.f);
	uniform vec3 u_lightDirection = vec3(-1.f, -1.f, -1.f);  
	uniform float u_specularity = 1.f;
	/**
	 * @brief Get directional light
	 * 
	 * @param color 
	 * @param direction 
	 * @return vec3 directional light value
	 */
	vec3 directionalLight(in vec3 color, in vec3 direction) {
		float ambientStrength = 0.1f;
		vec3 ambient = ambientStrength * color;

		vec3 lightDir = normalize(-direction);

		vec3 diffuse = color * max(0.f, dot(vs_normal, lightDir));

		vec3 viewDirection = normalize(vec3(inverse(u_viewMatrix) * vec4(0, 0, 0, 1) - u_modelMatrix * vs_fragPos));

		vec3 reflectionDirection = reflect(lightDir, vs_normal);

		float specularPower = pow(max(0.f, dot(viewDirection, reflectionDirection)), 32);

		vec3 specular = u_specularity * specularPower * color;

		return ambient + diffuse + specular;
	}
    /**
     * Main fragment shader program.
     */
	void main() {
		vec3 light = directionalLight(u_lightColor, u_lightDirection);
		
		color = vec4(u_objectColor, 1.f);
		color = color * vec4(light, 1.f);
	}
)";
#endif
