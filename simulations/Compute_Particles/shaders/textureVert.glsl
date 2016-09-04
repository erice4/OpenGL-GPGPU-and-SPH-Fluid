#version 330 core

/// @brief Model transform matrix passed in from Transform Class in main app
uniform mat4 MV;
uniform mat4 MVP;

layout (location=0) in vec3 inPosition;
layout (location=1)in vec2 inUV;
out vec2 vertUV;
uniform int xMultiplyer=1;

out vec3 vPosition;

void main()
{
 // calculate the vertex position
 gl_Position = MVP*vec4(inPosition, 1.0);
 // get the texture co-ord and mutliply it by the texture matrix
 vertUV=inUV.st;
 vertUV.s *=xMultiplyer;
 vertUV.t *=xMultiplyer;

 vec4 vertexPos = MV * vec4(inPosition, 1.0);
 vec3 vertexEyePos = vertexPos.xyz / vertexPos.w;

 vPosition = vertexEyePos;

}
