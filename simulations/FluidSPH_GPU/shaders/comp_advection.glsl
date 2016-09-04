#version 430 core
#extension GL_ARB_compute_shader:               enable
#extension GL_ARB_shader_storage_buffer_object: enable
#extension GL_ARB_compute_variable_group_size:  enable

layout (std430, binding=0) buffer Pos
{
  vec4 position[ ];
};

layout (std430, binding=1) buffer PrevPos
{
  vec4 prevPosition[ ];
};

layout (std430, binding=2) buffer Vel
{
  vec4 velocity[ ];
};

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

uniform float dt;
uniform float gravity;

void main()
{
    uint index = gl_GlobalInvocationID.x;

    velocity[index].y += gravity * dt;

    prevPosition[index] = position[index];
    position[index] += velocity[index] * dt;
}
