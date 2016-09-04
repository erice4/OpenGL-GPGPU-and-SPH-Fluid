#version 420 core

in vec2 vertUV;
in vec4 perVertexColor;

out layout (location=0) vec4 fragColor;

uniform sampler2D tex;

void main ()
{
    fragColor = texture(tex,vertUV);

    if (fragColor.r == 0 && fragColor.g == 0 && fragColor.b == 0)
    {
        discard;
    }
}
