#version 430 core

layout (location=0) in vec4 inPosition;
layout (location=1) in vec4 inColor;

uniform mat4 VP;

out vec3 color;

void main(void)
{
    gl_Position = VP * vec4(inPosition);
    //color = vec3(inPosition.y, abs(inPosition.y), 0.6);
    //color = vec3(0.8,mag,abs(inColor.x)); //Bluey green
    //color = vec3(mag,0.8,abs(inColor.x)); // Creamey
    //color = vec3(mag,mag,abs(inColor.x));
    vec3 c = normalize(inColor).xyz;
    float magXZ = length(vec2(c.x,c.z));
    //float mag = length(c);
    //color = vec3(0.4+abs(c.x)*2,0.4+abs(c.x)*2,0.95f);
    color = vec3(1,magXZ+0.3f,magXZ);
    //color = vec3(1-abs(c.y),1.1-abs(c.y),0.9);
}
