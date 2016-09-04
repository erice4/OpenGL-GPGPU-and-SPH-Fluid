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

/*layout (std140, binding=3) buffer Spheres
{
  vec4 spheres[ ];
};*/

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

uniform float dt;
uniform float particleRadius;
uniform float halfBoundsSize;
uniform vec3 boundsCenter;

struct wall
{
    float nx;
    float ny;
    float length;
};

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
    uint index = gl_GlobalInvocationID.x;

    velocity[index] = (position[index] - prevPosition[index]) / dt;

    vec3 pos = position[index].xyz;
    vec3 vel = velocity[index].xyz;

    float bounce = 0.26f;


    if (pos.x - particleRadius < boundsCenter.x - halfBoundsSize)
    {
        vec3 wallNormal = vec3(1,0,0);
        vel = vel - 2 * dot(vel, wallNormal) * wallNormal * bounce;
        pos.x = boundsCenter.x - halfBoundsSize + particleRadius;
    }
    else if (pos.x + particleRadius > boundsCenter.x + halfBoundsSize)
    {
        vec3 wallNormal = vec3(-1,0,0);
        vel = vel - 2 * dot(vel, wallNormal) * wallNormal * bounce;
        pos.x = boundsCenter.x + halfBoundsSize - particleRadius;
    }

    if (pos.y - particleRadius < boundsCenter.y - halfBoundsSize)
    {
        vec3 wallNormal = vec3(0,1,0);
        vel = vel - 2 * dot(vel, wallNormal) * wallNormal * bounce;
        pos.y = boundsCenter.y - halfBoundsSize + particleRadius;
    }
    else if (pos.y + particleRadius > boundsCenter.y + halfBoundsSize)
    {
        vec3 wallNormal = vec3(0,-1,0);
        vel = vel - 2 * dot(vel, wallNormal) * wallNormal * bounce;
        pos.y = boundsCenter.y + halfBoundsSize - particleRadius;
    }

    if (pos.z - particleRadius < boundsCenter.z - halfBoundsSize)
    {
        vec3 wallNormal = vec3(0,0,1);
        vel = vel - 2 * dot(vel, wallNormal) * wallNormal * bounce;
        pos.z = boundsCenter.z - halfBoundsSize + particleRadius;
    }
    else if (pos.z + particleRadius > boundsCenter.z + halfBoundsSize)
    {
        vec3 wallNormal = vec3(0,0,-1);
        vel = vel - 2 * dot(vel, wallNormal) * wallNormal * bounce;
        pos.z = boundsCenter.z + halfBoundsSize - particleRadius;
    }

    position[index].xyz = pos;
    velocity[index].xyz = vel;
}
