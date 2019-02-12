#include "Main.h"
#include "ObjLoader.h"

Renderer3D::Renderer3D(Window* in) : window(in) {}

Renderer3D::~Renderer3D()
{
  delete glslProcessor;
  delete batch3D;
  delete camera3D;
  delete keyProcessing;
  delete mesh;
}

#include <iostream>
void Renderer3D::initialize()
{
  glEnable(GL_DEPTH_TEST);

  glslProcessor = new GlslProcessor;
  batch3D = new Batch3D;

  camera3D = new Camera3D;
  camera3D->init(glm::vec3( 0.0f, 0.0f, -5.0f ));

  glslProgramId = glslProcessor->makeNewGlslProgram({
    glslProcessor->loadShaderFile( "shaders/Vertex.glsl"   , GL_VERTEX_SHADER  ),
    glslProcessor->loadShaderFile( "shaders/Fragment.glsl" , GL_FRAGMENT_SHADER)
  });

  ObjLoader objLoader;
  mesh = objLoader.loadMesh("examples/Anvil.obj");

  glUseProgram(glslProgramId);
  {
    batch3D->generateBuffers();
    batch3D->mapBuffersInformation();
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

  glUseProgram(glslProgramId);
  {
    batch3D->draw(glslProcessor, camera3D, glslProgramId, mesh);
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
