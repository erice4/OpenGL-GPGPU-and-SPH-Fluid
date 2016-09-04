#version 420 core


layout (location=0) in vec3 inPosition;
layout (location=1) in vec3 inColor;
layout (location=2) in vec3 inVel;

out vec3 color;

void main(void)
{
  gl_Position = vec4(inPosition,1.0f);
  //float den = (inColor.y/10.0f);
  float den = (inColor.y) / 13.5f;
  //color = vec3(0.2f,1-den,1.3f-den);
  color = vec3(1.3f-den,1.6f-den,1.8f-den);
  //color = vec3(0.2f,0.7f-den,1-den);
}
