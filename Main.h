#ifndef MAIN_H
#define MAIN_H

#include "Window.h"
#include "GlslProcessor.h"
#include "ModelBatch.h"
#include "LightBatch.h"
#include "Camera3D.h"
#include "KeyProcessing.h"

class Renderer3D
{
public:

  ~Renderer3D();

  Renderer3D(Window* in);

  /**
   * Initializes all the components for rendering.
   * e.g. The batch, Camera, GlslProcessor ...
   */
  void initialize();

  /**
   * Called once a frame to perform input.
   */
  void update();

  /**
   * Called once a frame to render to the screen.
   */
  void render();

private:
  Window*        window;
  GlslProcessor* glslProcessor;
  ModelBatch*    modelBatch;
  LightBatch*    lightBatch;
  Camera3D*      camera3D;
  KeyProcessing* keyProcessing;

  Light light;

  Mesh* mesh;

  GLuint modelProgramId;
  GLuint lightProgramId;
};

#endif
