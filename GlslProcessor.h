#ifndef GLSL_PROCESSOR_H
#define GLSL_PROCESSOR_H

#include <GL/glew.h>
#include <vector>
#include <functional>

class GlslProcessor
{
public:

  ~GlslProcessor();

  /**
   * Takes already compiled shader Ids and links them
   * into a GLSL program.
   */
  GLuint makeNewGlslProgram(std::vector<GLuint> shaderIds);

  /**
   * Compiles the GLSL shader file and returns it's Id.
   */
  GLuint loadShaderFile(
      const char* filePath,
      GLenum      shaderType);

  GLuint loadShaderFile(
      const char*                             filePath,
      GLenum                                  shaderType,
      std::function<std::string(std::string)> lineProcess);

  /**
   * Gets the uniform location in the GLSL program
   * based on it's name. The location is represented
   * as an int.
   */
  GLint getUniform(GLuint programId, const char* uniformName);

private:

  /**
   * The Id information from a compiled GLSL
   * program used for cleaning up at shutdown.
   */
  struct GlslProgram
  {
    GLuint              programId;
    std::vector<GLuint> shaderIds;
  };

  std::vector<GlslProgram> glslPrograms;

};

#endif
