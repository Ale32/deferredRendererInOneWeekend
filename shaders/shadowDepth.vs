#version 410 core
layout(location = 0) in vec3 inPosition;

uniform mat4 uModel;
uniform mat4 uViewProjection;

void main(){
    gl_Position = uViewProjection * uModel * vec4(inPosition, 1.0);
}