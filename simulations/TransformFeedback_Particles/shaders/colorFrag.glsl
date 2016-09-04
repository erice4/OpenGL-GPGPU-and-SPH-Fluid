#version 400 core

layout (location=0) out vec4 fragColor;

in vec3 color;

void main()
{
    vec3 c = color;

    if (c.r == 0 && c.g == 0 && c .b == 0)
    {
        c = vec3(1,1,1);
    }

    fragColor = vec4(c,1);
}
