#include "LightBatch.h"
#include "BatchUtils.h"

LightBatch::~LightBatch()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void LightBatch::generateBuffers()
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
}

void LightBatch::mapBuffersInformation()
{
  bindBatch();
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
  unbindBatch();
}

void LightBatch::uploadCameraUniforms(GLuint programId, GlslProcessor* glslProcessor, Camera3D* camera3D)
{
  BatchUtils::uploadCameraUniforms(programId, glslProcessor, camera3D);
}

void LightBatch::draw(GLuint programId, GlslProcessor* glslProcessor, std::vector<Light> lights)
{
  // Contribution by:
  // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
  // for the layout of the data.
  GLfloat cubeData[] = {
    -1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f, -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f, 1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f, -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, 1.0f
  };

  std::vector<GLfloat> allCubes;
  for (int i = 0; i < lights.size(); i++)
  {
    std::copy(cubeData, cubeData + sizeof(cubeData) / sizeof(*cubeData), std::back_inserter(allCubes));
  }

  // Scaling the lights to make them smaller than what is being rendered.
  glm::mat4 scalingMat = glm::scale(glm::mat4(1), glm::vec3(0.5f, 0.5f, 0.5f));

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, allCubes.size() * sizeof(GLfloat), &allCubes[0], GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);

  for (int i = 0; i < lights.size(); i++)
  {

    glm::mat4 translationMat = glm::translate(glm::mat4(1), lights[i].position);
    glm::mat4 modelMatrix = translationMat * scalingMat;

    glUniformMatrix4fv(
      glslProcessor->getUniform(programId, "modelMatrix"),
      1,
      GL_FALSE,
      glm::value_ptr(modelMatrix));

    glUniform3fv(glslProcessor->getUniform(programId, "lightColor"), 1, glm::value_ptr(lights[i].color));
    glDrawArrays(GL_TRIANGLES, i * 12 * 3, 12 * 3);
    
  }

  glDisableVertexAttribArray(0);
}

void LightBatch::bindBatch()
{
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void LightBatch::unbindBatch()
{
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
