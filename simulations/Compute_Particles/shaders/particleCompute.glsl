#version 430 core
#extension GL_ARB_compute_shader:               enable
#extension GL_ARB_shader_storage_buffer_object: enable
#extension GL_ARB_compute_variable_group_size:  enable


layout (std140, binding=4) buffer Pos
{
  vec4 positions[ ];
};

layout (std140, binding=5) buffer InitPos
{
  vec4 initialPositions[ ];
};

layout (std140, binding=6) buffer Vel
{
  vec4 velocities[ ];
};

layout (std140, binding=7) buffer Life
{
  vec4 lifetimes[ ];
};


//layout (local_size_variable) in;
layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;


vec3 sphereCollision(vec3 ppos, vec3 pvel, vec3 spherePos, float radius)
{
    if (length(ppos-spherePos) < radius)
    {
        vec3 n = normalize(ppos-spherePos);
        vec3 vout = reflect(pvel,n)*0.6f;

        return vout;
    }
    else
    {
        return pvel;
    }
}

void main()
{
    vec3 spherePos1 = vec3(0,-80,0);
    vec3 spherePos2 = vec3(-30,-140,30);
    vec3 spherePos3 = vec3(20,-170,40);
    vec3 spherePos4 = vec3(0,-200,50);
    vec3 spherePos5 = vec3(-50,-210,52);
    vec3 spherePos6 = vec3(45,-205,55);

    uint index = gl_GlobalInvocationID.x;
    vec3 p = positions[index].xyz;
    vec3 v = velocities[index].xyz;


    float dt = 0.15f;
    vec3 extForce = vec3(0, -1.8f, 0);
    extForce += -0.1f * v; //Drag

    v += dt * extForce; //Should be extForce / mass


    //Check is hitting a sphere and resolve collision
    v = sphereCollision(p,v,spherePos1, 50);
    v = sphereCollision(p,v,spherePos2, 50);
    v = sphereCollision(p,v,spherePos3, 60);
    v = sphereCollision(p,v,spherePos4, 70);
    v = sphereCollision(p,v,spherePos5, 72);
    v = sphereCollision(p,v,spherePos6, 65);

    p += dt * v;

    positions[index].xyz = p;
    velocities[index].xyz = v;

    lifetimes[index].x -= dt;

    if (lifetimes[index].x < 0)
    {
        lifetimes[index].x = lifetimes[index].y;
        positions[index].xyz = initialPositions[index].xyz;
        velocities[index] = vec4(velocities[index].w,0,velocities[index].w,1);
    }
}


