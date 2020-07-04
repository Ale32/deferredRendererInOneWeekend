#version 410 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

out vec2 UV;

void main() {
	UV = inUV;
    gl_Position = vec4(inPosition, 1.0);
}
