#ifndef BATCH_UTILS_H
#define BATCH_UTILS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GlslProcessor.h"
#include "Camera3D.h"

namespace BatchUtils
{

void uploadCameraUniforms(GLuint programId, GlslProcessor* glslProcessor, Camera3D* camera3D);

}

#endif
