#include "ComputeShader.h"
#include <iostream>


namespace doGPU
{
  ComputeShader::ComputeShader()
  {
    
  }
  
  ComputeShader::~ComputeShader()
  {
    
  }
  
  void ComputeShader::initialize(std::string _shaderFilePath, unsigned int _numWorkingGroups)
  {
    m_computeProgramName = "ComputeProgram";
    m_shaderManager.createShaderProgram(m_computeProgramName);
    m_shaderManager.addShaderFromFileToProgram(m_computeProgramName, doGPU::ShaderType::compute, _shaderFilePath);
    m_shaderManager.linkProgram(m_computeProgramName);
    m_shaderManager.useProgram(m_computeProgramName);

    m_numWorkingGroups = _numWorkingGroups;
  }

  void ComputeShader::createSSBO(std::string _bufferName, GLsizeiptr _dataSize, GLuint _binding)
  {
    SSBO ssbo;
    ssbo.createSSBO(_dataSize, _binding);

    m_ssbos[_bufferName] = ssbo;
  }

  void* ComputeShader::mapDataToBuffer(std::string _bufferName, GLsizeiptr _dataSize)
  {
    if (findSSBO(_bufferName))
    {
      return m_ssbos[_bufferName].mapDataToBuffer(_dataSize);
    }
    else
    {
      std::cerr<<"Cannot map to buffer\n";
      return NULL;
    }
  }

  void ComputeShader::unmapBuffer()
  {
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  }

  GLuint ComputeShader::getSSBO(std::string _bufferName)
  {
    if (findSSBO(_bufferName))
    {
      return m_ssbos[_bufferName].getBufferID();
    }
    else
    {
      std::cerr<<"WARNING: Could not find buffer, returning 0\n";
      return 0;
    }
  }

  GLuint ComputeShader::getComputePrgram()
  {
    return m_shaderManager.getProgramID(m_computeProgramName);
  }

  void ComputeShader::useProgram()
  {
    m_shaderManager.useProgram(m_computeProgramName);
  }

  void ComputeShader::dispatch()
  {
    for (auto buffer : m_ssbos)
    {
      buffer.second.bindBufferToLocation();
    }

    glDispatchCompute(m_numWorkingGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glUseProgram(0);
  }

  bool ComputeShader::findSSBO(std::string _bufferName)
  {
    auto program = m_ssbos.find(_bufferName);

    if (program != m_ssbos.end())
    {
      return true;
    }
    else
    {
      std::cerr<<"ERROR: SSBO " << _bufferName.c_str() << " cannot be found.\n";
      return false;
    }
  }

  /// Code for atomic counter and for getting back a pointer to the buffer. Add in later.
  /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_posSSBO);
  GLfloat *ptr2;
  ptr2 = (GLfloat*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
  GLfloat v1 = ptr2[4];
  GLfloat v2 = ptr2[5];
  GLfloat v3 = ptr2[6];

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  std::cout<<v1<<", "<<v2<<", "<<v3<<"\n";*/


  //emitParticles();
 /*GLuint countBuffer;
  glGenBuffers( 1, &countBuffer);
  glBindBufferBase( GL_ATOMIC_COUNTER_BUFFER, 7, countBuffer);
  glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
  GLuint zero = 0;
  glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_posSSBO);*/

  //m_compute.use();

  // Set memory barrier on per vertex base to make sure we get what was written by the compute shaders
  /*glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
  //glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
  //glMemoryBarrier(GL_ALL_BARRIER_BITS);


  glBindBuffer(GL_SHADER_STORAGE_BUFFER, countBuffer);
  GLuint *ptr = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
  GLuint pc = ptr[0];
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/

  //std::cout<<pc<<"\n";

  /// Map and unmap buffer so data can be edited
  /*vec4* positions = m_positions;
  GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
  m_positions = reinterpret_cast<vec4*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,m_maxParticles * sizeof(vec4), bufMask));


  for (int i=0; i<m_maxParticles; i++)
  {
    m_positions[i] = positions[i];
    m_positions[i].y += 0.01f;
  }

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/
}
