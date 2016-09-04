#include "TransformFeedback.h"
#include <iostream>

namespace doGPU
{
  TransformFeedback::TransformFeedback()
  {

  }

  TransformFeedback::~TransformFeedback()
  {

  }

  void TransformFeedback::initialize(std::string _shaderFilePath, int _numVaryings, const GLchar** _varyings)
  {
    m_shaderName = "TFShader";
    m_shaderManager.createShaderProgram(m_shaderName);
    m_shaderManager.addShaderFromFileToProgram(m_shaderName,doGPU::vertex,_shaderFilePath);

    glTransformFeedbackVaryings (m_shaderManager.getProgramID(m_shaderName), _numVaryings, _varyings,GL_INTERLEAVED_ATTRIBS);

    m_shaderManager.linkProgram(m_shaderName);
    m_shaderManager.useProgram(m_shaderName);
  }

  void TransformFeedback::addVertexAttribute(unsigned int _id, unsigned int _numFloats, GLenum _type, bool _normalize, GLsizei _stride, unsigned int _numFloatsToOffset)
  {
    vertexAttribute v;

    v.id = _id;
    v.numFloats = _numFloats;
    v.type = _type;
    v.normalize = _normalize;
    v.stride = _stride;
    v.numFloatsToOffset = _numFloatsToOffset;

    std::cout<<"adding vertex attribute\n";
    m_vertexAttributes.push_back(v);
  }

  void TransformFeedback::setData(const GLfloat &_data, unsigned int _numIndices, GLsizeiptr _totalDataSize)
  {
    m_numIndices = _numIndices;

    //VAO for transform feedback
    glGenVertexArrays(1,&m_VAO);
    glBindVertexArray(m_VAO);

    //VBOs for transform feedback
    glGenBuffers(1,&m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, _totalDataSize,&_data, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1,&m_feedbackVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_feedbackVBO);
    glBufferData(GL_ARRAY_BUFFER, _totalDataSize, nullptr, GL_STREAM_DRAW);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_feedbackVBO);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void TransformFeedback::useProgram()
  {
    m_shaderManager.useProgram(m_shaderName);
  }

  void TransformFeedback::feedback()
  {
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindVertexArray(m_VAO);

    for (unsigned int i=0; i<m_vertexAttributes.size(); i++)
    {
      glEnableVertexAttribArray(m_vertexAttributes[i].id);
      glVertexAttribPointer(m_vertexAttributes[i].id, m_vertexAttributes[i].numFloats, m_vertexAttributes[i].type, m_vertexAttributes[i].normalize, m_vertexAttributes[i].stride,(GLvoid*)(sizeof(GLfloat)*m_vertexAttributes[i].numFloatsToOffset));
    }

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_feedbackVBO);

    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS,0,m_numIndices);
    glEndTransformFeedback();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisable(GL_RASTERIZER_DISCARD);
    std::swap(m_VBO, m_feedbackVBO);

  }
}
