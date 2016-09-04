#version 420 core


layout (location=0) in vec3 inPosition;
layout (location=1) in vec3 inColor;
layout (location=2) in vec3 inVel;

out vec3 color;

void main(void)
{
  gl_Position = vec4(inPosition,1.0);
  color = inColor;
}
