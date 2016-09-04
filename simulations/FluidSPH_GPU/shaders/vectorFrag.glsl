#version 400 core

layout (location=0) out vec4 fragColor;

in vec4 perVertexColor;

void main()
{
    fragColor = perVertexColor;
    //fragColor = vec4(1,0,0,1);
}
