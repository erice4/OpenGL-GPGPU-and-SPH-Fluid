#version 330 core

uniform sampler2D tex;
uniform vec3 lightPosition;
uniform vec3 lightDirection;
in vec2 vertUV;
layout (location=0) out vec4 fragColor;
void main ()
{
    /*float dotNormalVP;
    float pf = 0;
    float spotDot;
    float spotAttenuation;
    float attenuation;
    float dist; //distance from surface to light position
    vec3 VP; //direction from surface to light position

    VP = lightPosition - vPosition;
    d = length(VP);
    VP = normalize(VP);

    attenuation = 1.0f;
    spotDot = dot(-VP, normalize(lightDirection));

    if (spotDot < )*/

    fragColor = texture(tex,vertUV);
}
