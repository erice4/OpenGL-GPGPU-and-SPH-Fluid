#ifndef COMPUTESHADER
#define COMPUTESHADER

#include <GL/glew.h>
#include "BaseGPGPU.h"
#include "ShaderManager.h"
#include "SSBO.h"
#include <unordered_map>
#include <string>

namespace doGPU
{
  class ComputeShader
  {

  public:
    ComputeShader();
    ~ComputeShader();

    void initialize(std::string _shaderFilePath, unsigned int _numWorkingGroups);

    void createSSBO(std::string _bufferName, GLsizeiptr _dataSize, GLuint _binding);
    void* mapDataToBuffer(std::string _bufferName, GLsizeiptr _dataSize);
    void unmapBuffer();

    GLuint getSSBO(std::string _bufferName);
    GLuint getComputePrgram();

    void useProgram();
    void dispatch();

    void setWorkingGroups( unsigned int _numWorkingGroups){m_numWorkingGroups = _numWorkingGroups;}

  private:

    bool findSSBO(std::string _bufferName);

    GLuint m_numWorkingGroups;
    std::unordered_map <std::string, SSBO> m_ssbos;

    ShaderManager m_shaderManager;
    std::string m_computeProgramName;
  };
}

#endif // COMPUTE

