#version 420 core
/// @brief Class originally written by Jon Macey and then modified.

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 V;
uniform mat4 VP;

in vec3 color[];

out vec4 perVertexColor;
out vec2 vertUV;

void main()
{
    float bbWidth = 0.25f;
    float bbHeight = bbWidth;

    vec3 right = vec3(V[0][0], V[1][0], V[2][0]);
    vec3 up = vec3(V[0][1], V[1][1], V[2][1]);

    vec3 pos = gl_in[0].gl_Position.xyz;

    //Bottom Left
    vec3 p1 = pos - (right + up) * bbWidth;
    gl_Position = VP * vec4(p1,1.0);
    vertUV = vec2(0.0, 0.0);
    perVertexColor = vec4(color[0],1);
    EmitVertex();

    //Top left
    vec3 p2 = pos - (right - up) * bbWidth;
    gl_Position = VP * vec4(p2, 1.0);
    vertUV = vec2(0.0, 1.0);
    perVertexColor = vec4(color[0],1);
    EmitVertex();

    //Top Right
    vec3 p3 = pos + (right - up) * bbWidth;
    gl_Position = VP * vec4(p3, 1.0);
    vertUV = vec2(1.0, 0.0);
    perVertexColor = vec4(color[0],1);
    EmitVertex();

    //Bottom right
    vec3 p4 = pos + (right + up) * bbWidth;
    gl_Position = VP * vec4(p4, 1.0);
    vertUV = vec2(1.0, 1.0);
    perVertexColor = vec4(color[0],1);
    EmitVertex();

    EndPrimitive();
}
