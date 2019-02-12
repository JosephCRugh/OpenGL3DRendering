#include "Main.h"
#include "ObjLoader.h"

Renderer3D::Renderer3D(Window* in) : window(in) {}

Renderer3D::~Renderer3D()
{
  delete glslProcessor;
  delete modelBatch;
  delete camera3D;
  delete keyProcessing;
  delete mesh;
  delete lightBatch;
}

#include <iostream>
void Renderer3D::initialize()
{
  glEnable(GL_DEPTH_TEST);

  glslProcessor = new GlslProcessor;
  modelBatch = new ModelBatch;
  lightBatch = new LightBatch;

  camera3D = new Camera3D;
  camera3D->init(glm::vec3( 0.0f, 0.0f, -5.0f ));

  modelProgramId = glslProcessor->makeNewGlslProgram({
    glslProcessor->loadShaderFile( "shaders/ModelVertex.glsl"   , GL_VERTEX_SHADER  ),
    glslProcessor->loadShaderFile( "shaders/ModelFragment.glsl" , GL_FRAGMENT_SHADER)
  });

  lightProgramId = glslProcessor->makeNewGlslProgram({
    glslProcessor->loadShaderFile( "shaders/LightVertex.glsl"   , GL_VERTEX_SHADER  ),
    glslProcessor->loadShaderFile( "shaders/LightFragment.glsl" , GL_FRAGMENT_SHADER)
  });

  ObjLoader objLoader;
  mesh = objLoader.loadMesh("examples/Anvil.obj");

  light.position = { 0.0f, 5.0f, 0.0f };
  light.color = { 1.0f, 0.0f, 0.0f };

  glUseProgram(modelProgramId);
  {
    modelBatch->generateBuffers();
    modelBatch->mapBuffersInformation();
  }
  glUseProgram(0);

  glUseProgram(lightProgramId);
  {
    lightBatch->generateBuffers();
    lightBatch->mapBuffersInformation();
  }
  glUseProgram(0);

  keyProcessing = new KeyProcessing(window, camera3D);
  keyProcessing->invokeProcessing();
}

void Renderer3D::update()
{
  glfwPollEvents();

  keyProcessing->transformCamera();

  camera3D->updateViewMatrix();
}

void Renderer3D::render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(lightProgramId);
  {
    lightBatch->bindBatch();
    lightBatch->uploadCameraUniforms(lightProgramId, glslProcessor, camera3D);
    lightBatch->draw(lightProgramId, glslProcessor, light);
    lightBatch->unbindBatch();
  }
  glUseProgram(0);

  glUseProgram(modelProgramId);
  {
    modelBatch->bindBatch();
    modelBatch->uploadCameraUniforms(modelProgramId, glslProcessor, camera3D);
    modelBatch->uploadLightUniforms(modelProgramId, glslProcessor, light);
    modelBatch->draw(modelProgramId, glslProcessor, mesh);
    modelBatch->unbindBatch();
  }
  glUseProgram(0);

  window->swapBuffers();
}

int main(int argc, char const *argv[])
{
  Window* window = new Window;
  Renderer3D* renderer3D = new Renderer3D(window);

  window->create(1024, 768, "Renderer3D");
  renderer3D->initialize();

  window->run(&Renderer3D::update, &Renderer3D::render, *renderer3D);

  delete renderer3D;
  delete window;

  return 0;
}
