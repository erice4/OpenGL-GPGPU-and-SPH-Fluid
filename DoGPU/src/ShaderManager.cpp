#include "ShaderManager.h"
#include <fstream>
#include <iostream>

/*#ifdef TARGET_OS_MAC
    #include <gl.h>
    #include <glu.h>
#elif defined __linux
    #include <GL/gl.h>
    #include <GL/glu.h>
#elif WIN32
    #include <GL/gl.h>
    #include <GL/glu.h>
#else
    #error "Platform Unknown"
#endif*/

namespace doGPU
{
  ShaderManager::ShaderManager()
  {

  }

  ShaderManager::~ShaderManager()
  {
    for (auto program : m_shaderPrograms)
    {
      glDeleteProgram(program.second);
    }
  }

  void ShaderManager::createShaderProgram(std::string _programName)
  {
    GLuint programID;
    programID = glCreateProgram();

    m_shaderPrograms[_programName] = programID;
  }

  void ShaderManager::addShaderFromFileToProgram(std::string _programName, ShaderType _type, std::string _sourcePath)
  {
    if (findStringInMap(_programName))
    {
      GLuint shaderID = glCreateShader(getGLShaderType(_type));

      //------------ Attach Source ---------------//
      std::ifstream file(_sourcePath.c_str());

      std::string src;

      if (file.is_open())
      {
        /// @brief The following section is from :-
        /// Jon Macey, NGL

        //Add shader source code to the string
        src = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        src += '\0'; // Terminator needed for glShaderSource
        const char* data = src.c_str();
        glShaderSource(shaderID, 1, &data, NULL);

        /// End of citation
        std::cout<<"Source: " << _sourcePath.c_str() << " attached.\n";
      }
      else
      {
        std::cerr<< _sourcePath.c_str() << " not found\n";
        exit(EXIT_FAILURE);
      }

      //------------ Compile shader -------------//
      GLint result = GL_FALSE;
      GLint infoLogLength;

      glCompileShader(shaderID);

      glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
      glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
      if (infoLogLength == GL_FALSE)
      {
        std::cerr<<"Shader compile error and or warnings\n";
        printInfoLog(shaderID);
        exit(EXIT_FAILURE);
      }
      std::cout<<"Source: " << _sourcePath.c_str() << " compiled.\n";

      //--------- Attach shader to program ----------//
      glAttachShader(m_shaderPrograms[_programName], shaderID);
      std::cout<<"Source: " << _sourcePath.c_str() << " attached to program.\n";

      glDeleteShader(shaderID);
    }
    else
    {
      std::cerr<<"Cannot add shader to program " << _programName.c_str() << "\n";
    }
  }

  void ShaderManager::linkProgram(std::string _programName)
  {
    if (findStringInMap(_programName))
    {
      GLint infoLogLength;

      glLinkProgram(m_shaderPrograms[_programName]);

      glGetProgramiv(m_shaderPrograms[_programName], GL_INFO_LOG_LENGTH, &infoLogLength);

      /// @brief The following section is from :-
      /// Jon Macey, NGL
      if (infoLogLength > 0)
      {
        char *infoLog = new char[infoLogLength];
        GLint charsWritten  = 0;

        glGetProgramInfoLog(m_shaderPrograms[_programName], infoLogLength, &charsWritten, infoLog);

        std::cerr<<infoLog<<std::endl;
        delete [] infoLog;
        glGetProgramiv(m_shaderPrograms[_programName], GL_LINK_STATUS,&infoLogLength);
        if( infoLogLength == GL_FALSE)
        {
          std::cerr<<"Program link failed exiting \n";
          exit(EXIT_FAILURE);
        }
      }
      /// end of citation
    }
    else
    {
      std::cerr<<"Cannot link program " << _programName.c_str() << "\n";
    }
  }

  void ShaderManager::useProgram(std::string _programName)
  {
    if (findStringInMap(_programName))
    {
      glUseProgram(m_shaderPrograms[_programName]);
    }
    else
    {
      std::cerr<<"Cannot use program " << _programName.c_str() << "\n";
    }
  }

  GLuint ShaderManager::getProgramID(std::string _programName)
  {
    if (findStringInMap(_programName))
    {
      return m_shaderPrograms[_programName];
    }
    else
    {
      std::cerr<<"WARNING: Could not find program, returning 0\n";
      return 0;
    }
  }

  GLenum ShaderManager::getGLShaderType(ShaderType _type)
  {
    switch (_type)
    {
      case ShaderType::vertex :
      {
        return GL_VERTEX_SHADER;
        break;
      }
      case ShaderType::fragment :
      {
        return GL_FRAGMENT_SHADER;
        break;
      }
      case ShaderType::geometry :
      {
        return GL_GEOMETRY_SHADER;
        break;
      }
      case ShaderType::compute :
      {
        return GL_COMPUTE_SHADER;
        break;
      }
      default :
      {
        std::cerr<<"Default: vertex shader being attached.\n";
        return GL_VERTEX_SHADER;
      }
    }
  }

  /// Citation :- Jon Macey, NGL, https://github.com/NCCA/NGL/blob/master/src/Shader.cpp
  void ShaderManager::printInfoLog( const GLuint &_obj)
  {
    GLint infologLength = 0;
    GLint charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(_obj, GL_INFO_LOG_LENGTH,&infologLength);

    if(infologLength > 0)
    {
      infoLog = new char[infologLength];
      glGetShaderInfoLog(_obj, infologLength, &charsWritten, infoLog);

      std::cerr<<infoLog<<std::endl;
      delete [] infoLog;
    }
  }
  /// End of citation

  bool ShaderManager::findStringInMap(std::string _value)
  {
    auto program = m_shaderPrograms.find(_value);

    if (program != m_shaderPrograms.end())
    {
      return true;
    }
    else
    {
      std::cerr<<"ERROR: Shader program " << _value.c_str() << " cannot be found.\n";
      return false;
    }
  }
}



