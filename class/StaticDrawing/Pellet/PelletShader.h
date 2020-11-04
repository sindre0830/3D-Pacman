#ifndef __PELLETSHADER_H_
#define __PELLETSHADER_H_
/* library */
#include <string>
//vertex shader
static const std::string pelletVertexShader = R"(
#version 430 core
//input
layout(location = 0) in vec2 gridPos;
layout(location = 1) in float display;
//output
out float vs_display;

void main() {
    vs_display = display;
	gl_Position = vec4(gridPos, display, 1.f);
}
)";
//fragment shader
static const std::string pelletFragmentShader = R"(
#version 430 core
//input
in float vs_display;
//output
out vec4 color;

void main() {
    if(vs_display == -0.4f) {
        //set color white
        color = vec4(1.f, 1.f, 1.f, 1.f);
    } else {
        //set color black
        color = vec4(0.f, 0.f, 0.f, 1.f); 
    }   
}
)";

#endif
