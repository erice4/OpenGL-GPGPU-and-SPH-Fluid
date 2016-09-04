#version 420 core
/// @brief Class originally written by Jon Macey and then modified.

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 VP;
uniform vec3 cameraPosition;

in vec3 color[];

out vec4 perVertexColor;
out vec2 vertUV;

void main()
{
    float bbWidth = 1;
    float bbHeight = bbWidth;

    vec3 pos = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(cameraPosition - pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);
    right *= bbWidth;
    up *= bbHeight;
    //Bottom Left
    pos -= (right * 0.5);
    pos.z = gl_in[0].gl_Position.z;
    gl_Position = VP * vec4(pos,1.0);
    vertUV = vec2(0.0, 0.0);
    perVertexColor = vec4(color[0],1);
    EmitVertex();

    //Top left
    pos.y += 1;
    gl_Position = VP * vec4(pos, 1.0);
    vertUV = vec2(0.0, 1.0);
    perVertexColor = vec4(color[0],1);
    EmitVertex();

    //Bottom Right
    pos.y -= 1;
    pos += right;
    gl_Position = VP * vec4(pos, 1.0);
    vertUV = vec2(1.0, 0.0);
    perVertexColor = vec4(color[0],1);
    EmitVertex();

    //Top right
    pos.y += 1;
    gl_Position = VP * vec4(pos, 1.0);
    vertUV = vec2(1.0, 1.0);
    perVertexColor = vec4(color[0],1);
    EmitVertex();

    EndPrimitive();
}
