#ifndef __MAZESHADER_H_
#define __MAZESHADER_H_
/* library */
#include <string>
//2D vertex shader
static const std::string wallVertexShader = R"(
    #version 430 core
    /* input */
    layout(location = 0) in vec2 a_gridPos;
    layout(location = 1) in vec3 a_color;
    /* output */
    out vec3 vs_color;
    /* uniform */
    uniform mat4 u_collectionMatrix = mat4(1.f);
    /**
     * Main vertex shader program.
     */
    void main() {
        vs_color = a_color;
        gl_Position = u_collectionMatrix * vec4(a_gridPos, 0.5f, 1.f);
    }
)";
//2D fragment shader
static const std::string wallFragmentShader = R"(
    #version 430 core
    /* input */
    in vec3 vs_color;
    /* output */
    out vec4 color;
    /**
     * Main fragment shader program.
     */
    void main() {
        color = vec4(vs_color, 1.f);
    }
)";

//3D vertex shader
static const std::string wallVertexShader3D = R"(
    #version 430 core
    /* input */
    layout(location = 0) in vec3 a_gridPos;
    layout(location = 1) in vec2 a_texPos;
    /* output */
    out vec2 vs_texPos;
    /* uniform */
    uniform mat4 u_collectionMatrix = mat4(1.f);
    /**
     * Main vertex shader program.
     */
    void main() {
        vs_texPos = a_texPos;
        gl_Position = u_collectionMatrix * vec4(a_gridPos, 1.f);
    }
)";
//3D fragment shader
static const std::string wallFragmentShader3D = R"(
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
