#ifndef TRANSFORMFEEDBACK
#define TRANSFORMFEEDBACK

#include <GL/glew.h>
#include "ShaderManager.h"
#include <vector>

namespace doGPU
{
  class TransformFeedback
  {
  public:

    struct vertexAttribute
    {
      unsigned int id;
      unsigned int numFloats;
      GLenum type;
      bool normalize;
      GLsizei stride;
      unsigned int numFloatsToOffset;
    };

    TransformFeedback();
    ~TransformFeedback();

    /// brief
    /// Creates the shader program and attaches the shader at the file pathway _shaderFilePath,
    /// as well as setting up the shader to be used with Transform Feedback. _numVaryings should
    /// be the number of output variables in the shader, or the length of _varyings. _varyings
    /// is an array of a character arrays, which hold the names of the shader output variables.
    /// The names passed in through _varyings must match those in the shader.
    void initialize(std::string _shaderFilePath, int _numVaryings, const GLchar** _varyings);
    void addVertexAttribute(unsigned int _id, unsigned int _numFloats, GLenum _type, bool _normalize, GLsizei _stride, unsigned int _numFloatsToOffset);
    void setData(const GLfloat &_data, unsigned int _numIndices, GLsizeiptr _totalDataSize);

    void useProgram();
    void feedback();

    GLuint getVAO(){return m_VAO;}
    GLuint getVBO(){return m_VBO;}

  private:
    ShaderManager m_shaderManager;
    std::string m_shaderName;
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_feedbackVBO;

    std::vector<vertexAttribute> m_vertexAttributes;
    unsigned int m_numIndices;
  };
}

#endif // TRANSFORMFEEDBACK

