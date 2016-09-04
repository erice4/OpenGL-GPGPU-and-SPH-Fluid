#version 400 core

layout (location=0) in vec3 inPosition;
layout (location=1) in vec3 inColor;

uniform mat4 VP;

out vec3 color;

void main(void)
{
    gl_Position = VP * vec4(inPosition, 1);
    float den = inColor.z/8.4f;
    color = vec3(1-den,1.3-den,1.5-den);
}
