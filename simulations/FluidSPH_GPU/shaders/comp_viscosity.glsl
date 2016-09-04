#version 430 core
#extension GL_ARB_compute_shader:               enable
#extension GL_ARB_shader_storage_buffer_object: enable
#extension GL_ARB_compute_variable_group_size:  enable
#extension GL_NV_shader_atomic_float : enable

layout (std430, binding=0) buffer Pos
{
  vec4 position[ ];
};

layout (std430, binding=2) buffer Vel
{
  vec4 velocity[ ];
};

layout (std430, binding=7) buffer nGrid
{
  int grid[ ];
};

layout (std430, binding=8) buffer NeighB
{
  int neighbours[ ];
};

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

uniform float dt;
uniform float kernel;
uniform float linearViscosity;
uniform float quadraticViscosity;

uniform int numCellsPerAxis;
uniform int maxNeighbours;
uniform int maxParticlesPerCell;

void main()
{
    int totalCells = numCellsPerAxis*numCellsPerAxis*1;

//    for (int cc = 0; cc < totalCells; cc++)
//    {
        int sizeOfCell = maxParticlesPerCell + 4;
        int index = int(gl_GlobalInvocationID.x) * sizeOfCell; //This shader is being run once per cell, grid is of size pow(numCellsPerAxis, 3) * maxParticlesPerCell

        int numParticlesInCell = grid[index]; // The element at the start of the grid gives the number of particles in the cell
        int sizeOfNeighbourList = maxNeighbours + 1;

        for (int i=0; i<numParticlesInCell; i++)
        {
            int pIndex = grid[index+4+i]; //The +4 is to take into account that the 1st 4 elements of the cell are reserved for the number of particles in the cell.

            int nCount = 0;
            //Clear each particle's neighbours and set the first value to 0, as this represents the number of particles.

            for (int n=0; n<sizeOfNeighbourList; n++)
            {
                atomicExchange(neighbours[pIndex*sizeOfNeighbourList + n], -1);
            }
            atomicExchange(neighbours[pIndex*sizeOfNeighbourList], 0);
            for (int x=-1; x<2; x++)
            {
                for (int y=-1; y<2; y++)
                {
                    for (int z=-1; z<2; z++)
                    {
                        //Check if the cell being looked at is in bounds. Cell positions, x,y,z are stored in the 3 elements after the start of the cell.
                        if (grid[index+1] + x < 0 ||
                            grid[index+1] + x > numCellsPerAxis-1 ||
                            grid[index+2] + y < 0 ||
                            grid[index+2] + y > numCellsPerAxis-1 ||
                            grid[index+3] + z < 0 ||
                            grid[index+3] + z > numCellsPerAxis-1)
                        {
                            continue;
                        }
                        int xOffset = x * sizeOfCell;
                        int yOffset = y * sizeOfCell;
                        int zOffset = z * sizeOfCell;

                        int adjIndex = index + xOffset + (yOffset*numCellsPerAxis) + (zOffset*numCellsPerAxis*numCellsPerAxis);

                        int numParticlesInAdjCell = grid[adjIndex];
                        //Loop through all particles in the adjacent cell
                        for (int j=0; j<numParticlesInAdjCell; j++)
                        {
                            int npIndex = grid[(adjIndex + 4) + j];

                            //Clavet et. al viscosity calculation
                            vec3 dir = position[npIndex].xyz - position[pIndex].xyz;
                            float dist = length(dir);

                            if (dist < 0.00001f || dist > kernel)
                            {
                                continue;
                            }

                            vec3 dirNorm = normalize(dir);

                            float u = dot((velocity[pIndex].xyz - velocity[npIndex].xyz),dirNorm);

                            if (u > 0.0f)
                            {
                                float a = 1 - (dist/kernel);
                                vec3 impulse = dt * a * (linearViscosity * u + quadraticViscosity * u*u) * dirNorm *0.5f;

                                atomicAdd(velocity[pIndex].x, -impulse.x);
                                atomicAdd(velocity[pIndex].y, -impulse.y);
                                atomicAdd(velocity[pIndex].z, -impulse.z);

                                atomicAdd(velocity[npIndex].x, impulse.x);
                                atomicAdd(velocity[npIndex].y, impulse.y);
                                atomicAdd(velocity[npIndex].z, impulse.z);

                                //velocity[pIndex].xyz -= impulse;
                                //velocity[npIndex].xyz += impulse;
                            }
                            //neighbours[pIndex*sizeOfNeighbourList + nCount + 1] = npIndex; //+1 takes into account that the first element stores the number of particles
                            atomicExchange(neighbours[pIndex*sizeOfNeighbourList + nCount + 1], npIndex);
                            nCount++;
                            //neighbours[pIndex*sizeOfNeighbourList] = nCount; //Store number of particles
                            atomicExchange(neighbours[pIndex*sizeOfNeighbourList], nCount);

                            if (nCount >= maxNeighbours)
                            {
                                x = 2; y = 2; z = 2; //break out of all loops
                                break;
                            }
                          }
                    }
                }
            }
        }
    //}

}
