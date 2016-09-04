#version 420 core

in vec2 vertUV;
in vec4 perVertexColor;

out layout (location=0) vec4 fragColor;

uniform sampler2D tex;

void main ()
{
    vec4 texColor = texture(tex,vertUV);
    //texColor.xyz *= 0.1f;
    vec3 c = texColor.xyz * perVertexColor.xyz;
    fragColor =vec4(c,texColor.w);

    if (fragColor.r == 0 && fragColor.g == 0 && fragColor.b == 0)
    {
        discard;
    }
}
