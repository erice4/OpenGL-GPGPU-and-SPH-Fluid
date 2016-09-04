#ifndef SSBO_H
#define SSBO_H

#include <GL/glew.h>

namespace doGPU
{
  class SSBO
  {
  public:
    SSBO();
    ~SSBO();

    void deleteBuffer();

    void createSSBO(GLsizeiptr _dataSize, GLuint _binding);

    void* mapDataToBuffer(GLsizeiptr _dataSize);
    void unmapBuffer();
    void bindBuffer();
    void bindBufferToLocation();

    GLuint getBufferID(){return m_bufferID;}
    GLuint getBufferBinding(){return m_binding;}

    void setBufferBinding(GLuint _binding){m_binding=_binding;}

  private:
    GLuint m_bufferID;
    GLuint m_binding=0;
  };
}


#endif // SSBO_H
