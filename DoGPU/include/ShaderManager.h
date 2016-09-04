#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <unordered_map>
#include <string>
#include <GL/glew.h>

namespace doGPU
{

enum ShaderType {vertex, fragment, geometry, compute};

  class ShaderManager
  {

  public:
    //-----------------------------------------------------------------------------------------------------------------------------------
    ShaderManager();
    //-----------------------------------------------------------------------------------------------------------------------------------
    ~ShaderManager();

    //-----------------------------------------------------------------------------------------------------------------------------------
    void createShaderProgram(std::string _programName);
    //-----------------------------------------------------------------------------------------------------------------------------------
    void addShaderFromFileToProgram(std::string _programName, doGPU::ShaderType _type, std::string _sourcePath);
    //-----------------------------------------------------------------------------------------------------------------------------------
    void linkProgram(std::string _programName);
    //-----------------------------------------------------------------------------------------------------------------------------------
    void useProgram(std::string _programName);
    //-----------------------------------------------------------------------------------------------------------------------------------

    GLuint getProgramID(std::string _programName);
    //-----------------------------------------------------------------------------------------------------------------------------------

  private:
    //-----------------------------------------------------------------------------------------------------------------------------------
    GLenum getGLShaderType(doGPU::ShaderType _type);
    //-----------------------------------------------------------------------------------------------------------------------------------
    /// @brief Print shader info in case of errors and warnings.
    /// The following section is from :-
    /// Jon Macey, NGL https://github.com/NCCA/NGL/blob/master/include/ngl/Shader.h
    void printInfoLog(const GLuint &_obj);
    /// end of citation
    //-----------------------------------------------------------------------------------------------------------------------------------
    bool findStringInMap(std::string _value);
    //-----------------------------------------------------------------------------------------------------------------------------------
    std::unordered_map <std::string, GLuint> m_shaderPrograms;

  };

}



#endif // SHADERMANAGER_H
