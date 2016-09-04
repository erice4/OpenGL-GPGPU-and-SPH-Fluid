#version 430 core
#extension GL_ARB_compute_shader:               enable
#extension GL_ARB_shader_storage_buffer_object: enable
#extension GL_ARB_compute_variable_group_size:  enable
#extension GL_NV_shader_atomic_float : enable


layout  (std430, binding=9) buffer Debug
{
  float debug[ ];
};

shared float t;
shared float f;

layout (local_size_x = 16, local_size_y = 1, local_size_z = 1) in;

void main()
{
    ///////     1.     ///////
    uint index = gl_GlobalInvocationID.x;
    t = debug[0];
    memoryBarrierShared();
    barrier();
    t += 1;
    memoryBarrierShared();
    barrier();
    debug[0] = t;

    // Print debug[0]: 1


    ///////     2.     ///////

    //atomicAdd(debug[0], 1);
    //atomicExchange(debug[0], 3);
    // Print debug[0]: 16


    ///////     3.     ///////
    /*t = debug[0];
    memoryBarrierShared();
    barrier();
    t += 1;
    memoryBarrierShared();
    barrier();
    atomicExchange(debug[0], t);*/

    // Print debug[0]: 1
}
