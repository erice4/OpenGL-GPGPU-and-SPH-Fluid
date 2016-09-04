#version 430 core
#extension GL_ARB_compute_shader:               enable
#extension GL_ARB_shader_storage_buffer_object: enable
#extension GL_ARB_compute_variable_group_size:  enable

layout (std430, binding=7) buffer nGrid
{
  int grid[ ];
};

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

uniform int maxParticlesPerCell;

void main()
{
    uint index = gl_GlobalInvocationID.x;

    if (index % (maxParticlesPerCell+4) == 0)
    {
        grid[index] = 0;
    }
    else
    {
        grid[index] = -1;
    }

}
