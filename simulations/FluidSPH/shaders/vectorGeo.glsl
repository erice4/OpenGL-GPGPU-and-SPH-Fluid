#version 400 core

layout (points) in;
layout (line_strip) out;
layout (max_vertices = 2) out;

in vec4 vector[];
in vec3 color[];
out vec4 perVertexColor;

void main(void)
{
    vec4 pos = gl_in[0].gl_Position;
    vec4 dir = vector[0];
    //dir = vec4(0,1,0,0);
    float lineLength = 2.0;

    gl_Position = pos;
    //perVertexColor = vec4(vector[0].xyz,1);
    perVertexColor = vec4(1,1,1,1);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position+ dir*lineLength;

    //
    //perVertexColor = vec4(vector[0].xyz,1);
    perVertexColor = vec4(color[0],1);
    EmitVertex();

    EndPrimitive();
}
