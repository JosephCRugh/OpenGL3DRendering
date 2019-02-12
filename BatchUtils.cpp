#include "BatchUtils.h"

void BatchUtils::uploadCameraUniforms(GLuint programId, GlslProcessor* glslProcessor, Camera3D* camera3D)
{
  glUniformMatrix4fv(
    glslProcessor->getUniform(programId, "perspectiveMatrix"),
    1,
    GL_FALSE,
    glm::value_ptr(camera3D->getPerspectiveMatrix()));

  glUniformMatrix4fv(
    glslProcessor->getUniform(programId, "viewMatrix"),
    1,
    GL_FALSE,
    glm::value_ptr(camera3D->getViewMatrix()));
}
