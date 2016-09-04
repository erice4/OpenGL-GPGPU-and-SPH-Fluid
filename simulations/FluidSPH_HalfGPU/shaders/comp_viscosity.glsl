#version 430 core
#extension GL_ARB_compute_shader:               enable
#extension GL_ARB_shader_storage_buffer_object: enable
#extension GL_ARB_compute_variable_group_size:  enable


layout (std430, binding=0) buffer Pos
{
  vec4 position[ ];
};

layout (std430, binding=2) buffer Vel
{
  vec4 velocity[ ];
};

layout (std430, binding=7) buffer NeighB
{
  int neighbours[ ];
};

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

uniform float dt;
uniform float kernel;
uniform float linearViscosity;
uniform float quadraticViscosity;

const int MAX_NEIGHBOURS = 150;

void main()
{
    uint index = gl_GlobalInvocationID.x;

    //Run through all neighbouring particles. 20 is the max number of neighbours
    for (int i=0; i<MAX_NEIGHBOURS; i++)
    {
        int nIndex = neighbours[index*MAX_NEIGHBOURS+i];

        //If there are less than MAX_NEIGHBOURS neighbours to a particle, the index of the elements of neighbours after
        // the true number of neighbours will be set to -1 (during the neighbour search on the CPU) to indicate
        // there is no neighbouring particle in that element. Therefore we can exit the for loop as all neighbours
        // have been considered.
        if (nIndex < 0)
        {
            i=MAX_NEIGHBOURS;
            break;
        }

        //Clavet et. al viscosity calculation
        vec3 dir = position[nIndex].xyz - position[index].xyz;
        float dist = length(dir);

        if (dist <= 0.00001f)
        {
            continue;
        }

        vec3 dirNorm = normalize(dir);

        float u = dot((velocity[index].xyz - velocity[nIndex].xyz),dirNorm);

        if (u > 0.0f)
        {
            float a = 1 - (dist/kernel);
            vec3 impulse = dt * a * (linearViscosity * u + quadraticViscosity * u*u) * dirNorm;
            velocity[index].xyz -= impulse * 0.5f;
            velocity[nIndex].xyz += impulse * 0.5f;
        }
    }
}

/*layout (std140, binding=0) buffer Pos
{
  vec4 position[ ];
};

layout (std140, binding=1) buffer Vel
{
  vec4 velocity[ ];
};

layout (std140, binding=2) buffer Den
{
  float density[ ];
};

layout (std140, binding=3) buffer NearDen
{
  float nearDensity[ ];
};

layout (std140, binding=4) buffer Pressure
{
  float pressure[ ];
};

layout (std140, binding=5) buffer NearPressure
{
  float nearPressure[ ];
};

layout (std140, binding=6) buffer NeighB
{
  int neighbours[ ][20];
};*/
