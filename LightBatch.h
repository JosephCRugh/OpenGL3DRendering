#ifndef LIGHT_BATCH_H
#define LIGHT_BATCH_H

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "GlslProcessor.h"
#include "Camera3D.h"
#include "Geometry.h"

class LightBatch
{
public:

  ~LightBatch();

  /**
   * Generates the vertex array, and vertex buffer object.
   */
  void generateBuffers();

  /**
   * Maps the information on the CPU for the
   * attributes to the attribute on the GPU.
   */
  void mapBuffersInformation();

  /**
   * Uploads the camera's view and project matrices.
   */
  void uploadCameraUniforms(GLuint programId, GlslProcessor* glslProcessor, Camera3D* camera3D);

  /**
   * Binds the vertex array, and vertex buffer objects.
   */
  void bindBatch();

  /**
   * Unbinds the vertex array, and vertex buffer objects.
   */
  void unbindBatch();

  /**
   * Draws the light as a cube at the location specified
   * in the light with the color specified.
   */
  void draw(GLuint programId, GlslProcessor* glslProcessor, Light light);

private:
  GLuint vao;
  GLuint vbo;
};

#endif
