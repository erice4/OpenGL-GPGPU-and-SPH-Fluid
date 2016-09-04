#include "SSBO.h"
#include <iostream>
namespace doGPU
{
  SSBO::SSBO()
  {

  }

  SSBO::~SSBO()
  {

  }

  void SSBO::deleteBuffer()
  {
    glDeleteBuffers(1,&m_bufferID);
  }

  void SSBO::createSSBO(GLsizeiptr _dataSize, GLuint _binding)
  {
    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _dataSize, NULL, GL_DYNAMIC_COPY);

    m_binding = _binding;
  }

  void* SSBO::mapDataToBuffer(GLsizeiptr _dataSize)
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);

    GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
    return glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0,_dataSize, bufMask);
  }

  void SSBO::unmapBuffer()
  {
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  }

  void SSBO::bindBuffer()
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
  }

  void SSBO::bindBufferToLocation()
  {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding, m_bufferID);
  }
}


