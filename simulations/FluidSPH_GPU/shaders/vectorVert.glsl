#version 400 core

layout (location=0) in vec3 inPosition;
layout (location=1) in vec3 inVector;
layout (location=2) in vec3 inColor;

uniform mat4 VP;

out vec4 vector;
out vec3 color;

void main(void)
{
    gl_Position = VP * vec4(inPosition, 1);
    vector = VP * vec4(inVector,0);
    color = inColor;
}
