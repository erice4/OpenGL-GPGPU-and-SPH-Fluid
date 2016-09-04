#version 430 core
#extension GL_ARB_compute_shader:               enable
#extension GL_ARB_shader_storage_buffer_object: enable
#extension GL_ARB_compute_variable_group_size:  enable

layout (std430, binding=0) buffer Pos
{
  vec4 position[ ];
};

layout (std430, binding=7) buffer nGrid
{
  int grid[ ];
};

layout (local_size_x = 16, local_size_y = 1, local_size_z = 1) in;

uniform int numCellsPerAxis;
uniform float cellSize;
uniform vec3 gridOffset;
uniform int maxParticlesPerCell;

//Needs to be the size of the number of cells, though this size must be constant so remember to change this here if grid size changes on CPU side.
//shared int s_numParticles[16*16*16];
shared int sharedInt;

//Run for every particle
void main()
{
    uint index = gl_GlobalInvocationID.x;

    //Find index of the particle in the grid
    int x = int((position[index].x+gridOffset.x) / cellSize);
    int y = int((position[index].y+gridOffset.y) / cellSize);
    int z = int((position[index].z+gridOffset.z) / cellSize);

    if (x < 0) x=0;
    else if (x >= numCellsPerAxis) x = int(numCellsPerAxis) - 1;

    if (y < 0) y=0;
    else if (y >= numCellsPerAxis) y = int(numCellsPerAxis) - 1;

    if (z < 0) z=0;
    else if (z >= numCellsPerAxis) z = int(numCellsPerAxis) - 1;

    //Add the index of the particle to the cell. The first element at the cell index is reserved for the number of particles
    //that have been added to the cell. The next three elements hold the index positions of its place in the grid, as in it's x,y and z position.

    //To further explain the above - the neighboursGrid buffer reserves enough memory for the total number of cells * max number of particles
    //that may be added to a cell. The index of the start of a cell is at [x + y*numCellsPerAxis + z*numCellsPerAxis*numCellsPerAxis]. There is
    //then another MAX_PARTICLES_PER_CELL elements before the start of the next cell. These elements hold the index of particles currently in the cell.
    //All neighbourGrid elements except the first element of each cell have been set to -1 before this shader is called, which indicates there is no particle at that element.
    uint sizeOfCell = maxParticlesPerCell+4;
    uint cellIndex = (x * sizeOfCell + y * sizeOfCell*numCellsPerAxis + z * sizeOfCell*numCellsPerAxis*numCellsPerAxis);
    //uint numPIndex = (x + y *numCellsPerAxis + z *numCellsPerAxis*numCellsPerAxis);
    //s_numParticles[numPIndex] = grid[cellIndex];
    sharedInt = grid[0];

    memoryBarrierShared();
    barrier();

    //s_numParticles[numPIndex]+= 1;
    sharedInt += 1;

    atomicExchange(grid[cellIndex + 1], x);
    atomicExchange(grid[cellIndex + 2], y);
    atomicExchange(grid[cellIndex + 3], z);


    /*if (s_numParticles[numPIndex] < maxParticlesPerCell)
    {
        //particleGridIndex[index].w = 1;
        uint nextFreeElement = s_numParticles[numPIndex]+4; //It is +4 so we shift along by 4 elements, as [0] is being used for storing the number of particles in the cell, and
        //[1], [2] and [3] are used to store the position of the cell in the grid.
        atomicExchange(grid[cellIndex+nextFreeElement], int(index));
        s_numParticles[numPIndex] += 1;
    }*/
    memoryBarrierShared();
    barrier();
    //grid[cellIndex] = sharedInt;
    grid[0] = sharedInt;

}
