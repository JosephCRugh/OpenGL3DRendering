#include "GlslProcessor.h"

#include <fstream>
#include "Shutdown.h"

GlslProcessor::~GlslProcessor()
{
  for (GlslProgram glslProgram : glslPrograms)
  {
    for (GLuint shaderId : glslProgram.shaderIds)
    {
      glDeleteShader(shaderId);
    }
    glDeleteProgram(glslProgram.programId);
  }
}

GLuint GlslProcessor::makeNewGlslProgram(std::vector<GLuint> shaderIds)
{
  using namespace std;

  GLuint programId = glCreateProgram();
  for (GLuint shaderId : shaderIds)
  {
    glAttachShader(programId, shaderId);
  }

  glLinkProgram(programId);

  GLint isLinked = 0;
  glGetProgramiv(programId, GL_LINK_STATUS, (int*)&isLinked);
  if (isLinked == GL_FALSE)
  {
  	GLint maxLength = 0;
  	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

  	vector<GLchar> infoLog(maxLength);
  	glGetProgramInfoLog(
      programId,
      maxLength,
      &maxLength,
      &infoLog[0]);

    string errStr(infoLog.begin(), infoLog.end());
  	shutDown("%s\n", errStr.c_str());
  }

  for (GLuint shaderId : shaderIds)
  {
    glDetachShader(programId, shaderId);
  }

  GlslProgram glslProgram;
  glslProgram.programId = programId;
  glslProgram.shaderIds = shaderIds;
  glslPrograms.push_back(glslProgram);

  return programId;

}

GLint GlslProcessor::getUniform(GLuint programId, const char* uniformName)
{
  GLint location = glGetUniformLocation(programId, uniformName);
  if (location == GL_INVALID_INDEX)
  {
    shutDown("Failed to get uniform by name: %s", uniformName);
  }
  return location;
}

GLuint GlslProcessor::loadShaderFile(
                      const char* filePath,
                      GLenum      shaderType)
{
  using namespace std;

  ifstream shaderStream(filePath);
  if (!shaderStream)
  {
    shutDown([&filePath]() { perror( filePath ); });
  }

  string line = "", content;
  while(getline(shaderStream, line))
  {
    content.append(line + "\n");
  }

  shaderStream.close();

  GLuint shaderId = glCreateShader(shaderType);
  const GLchar* shaderSource = (const GLchar*) content.c_str();

  glShaderSource(shaderId, 1, &shaderSource, 0);
  glCompileShader(shaderId);

  GLint isCompiled = 0;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {
  	GLint maxLength = 0;
  	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

  	std::vector<GLchar> infoLog(maxLength);
  	glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

  	glDeleteShader(shaderId);

    string errStr(infoLog.begin(), infoLog.end());
  	shutDown("%s: %s\n", filePath, errStr.c_str());
  }

  return shaderId;
}
