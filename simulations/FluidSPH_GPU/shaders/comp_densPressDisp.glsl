#version 430 core
#extension GL_ARB_compute_shader:               enable
#extension GL_ARB_shader_storage_buffer_object: enable
#extension GL_ARB_compute_variable_group_size:  enable
#extension GL_NV_shader_atomic_float : enable


layout (std430, binding=0) buffer Pos
{
  vec4 position[ ];
};

layout (std430, binding=3) buffer Den
{
  float density[ ];
};

layout (std430, binding=4) buffer NearDen
{
  float nearDensity[ ];
};

layout (std430, binding=5) buffer Pressure
{
  float pressure[ ];
};

layout (std430, binding=6) buffer NearPressure
{
  float nearPressure[ ];
};

layout (std430, binding=8) buffer NeighB
{
  int neighbours[ ];
};

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

uniform float dt;
uniform float kernel;
uniform float restDensity;
uniform float stiffness;
uniform float nearStiffness;
uniform int maxNeighbours;

void main()
{
    uint index = gl_GlobalInvocationID.x;
    density[index] = 0;
    nearDensity[index] = 0;

    float dens = 0;
    float nearDens = 0;
    float press = 0;
    float nearPress = 0;

    int numNeighbours = neighbours[index*(maxNeighbours+1)];

    for (int i=0; i<numNeighbours; i++)
    {
        int nIndex = neighbours[index*(maxNeighbours + 1) + 1 + i];

        if (nIndex < 0)
        {
            break;
        }

        float dist = length(position[nIndex].xyz - position[index].xyz);

        float a = 1.0f - (dist/kernel);
        dens += a*a;
        nearDens += a*a*a;
    }

    press = (dens - restDensity) * stiffness;
    nearPress = nearDens * nearStiffness;

    float pressTotal = abs(press + nearPress);

    if (pressTotal < 0.00001f || pressTotal > 10000)
    {
        press = 0;
        nearPress = 0;
    }

    vec3 dispP = vec3(0,0,0);
    for (int i=0; i<maxNeighbours; i++)
    {
        int nIndex = neighbours[index*(maxNeighbours+1) + 1 + i];
        if (nIndex < 0)
        {
            break;
        }

        vec3 dir = position[nIndex].xyz - position[index].xyz;
        float dist = length(dir);
        float a = 1.0f - (dist/kernel);
        vec3 norm = normalize(dir);
        vec3 d = dt*dt * (press*a + nearPress*a*a) * norm;
        d *= 0.5f;
        atomicAdd(position[nIndex].x, d.x);
        atomicAdd(position[nIndex].y, d.y);
        atomicAdd(position[nIndex].z, d.z);
        dispP -= d;
    }
    //As position is being written to by other shader invocations must be an atomic add. Same as above.
    atomicAdd(position[index].x, dispP.x);
    atomicAdd(position[index].y, dispP.y);
    atomicAdd(position[index].z, dispP.z);

    density[index] = dens;
    nearDensity[index] = nearDens;
    pressure[index] = press;
    nearPressure[index] = nearPress;
}
