#include "Main.h"
#include "ObjLoader.h"
#include "Shutdown.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <streambuf>

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

void Renderer3D::initialize(const char* meshPath, const char* jsonFile)
{
  glEnable(GL_DEPTH_TEST);

  glslProcessor = new GlslProcessor;
  modelBatch = new ModelBatch;
  lightBatch = new LightBatch;
  camera3D = new Camera3D;

  loadJsonFile(jsonFile == NULL ? "Default.json" : jsonFile);

  modelProgramId = glslProcessor->makeNewGlslProgram({
    glslProcessor->loadShaderFile( "shaders/ModelVertex.glsl"   , GL_VERTEX_SHADER  ,
    [this](std::string line) {
      return (line.rfind("#define NUM_LIGHTS", 0) == 0) ?
        "#define NUM_LIGHTS " + std::to_string(lights.size()) + "\n"  :
        line + "\n";
    }),
    glslProcessor->loadShaderFile( "shaders/ModelFragment.glsl" , GL_FRAGMENT_SHADER)
  });

  lightProgramId = glslProcessor->makeNewGlslProgram({
    glslProcessor->loadShaderFile( "shaders/LightVertex.glsl"   , GL_VERTEX_SHADER  ),
    glslProcessor->loadShaderFile( "shaders/LightFragment.glsl" , GL_FRAGMENT_SHADER)
  });

  ObjLoader objLoader;
  mesh = objLoader.loadMesh(meshPath);

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

void Renderer3D::loadJsonFile(const char* jsonFile)
{
  std::ifstream inStream(jsonFile);

  if (!inStream)
  {
    shutDown([&jsonFile](){ perror(jsonFile); });
  }

  std::string contents((std::istreambuf_iterator<char>(inStream)),
                   std::istreambuf_iterator<char>());

  using json = nlohmann::json;
  auto root = json::parse(contents);

  // Loading in the camera values from the json file.
  auto jcamera = root["camera"];
  auto jcameraPos = jcamera["position"];

  camera3D->init(glm::vec3(jcameraPos[0], jcameraPos[1], jcameraPos[2]));

  float degrees = jcamera["rotation"];
  float radians = (degrees * 3.14159f) / 180.0f;
  camera3D->rotate(radians);

  // Loading in the light values from the json file.
  auto jlights = root["lights"];
  Light light;
  for (auto jlight : jlights)
  {
    auto jlightPos = jlight["position"];
    auto jlightColor = jlight["color"];
    light.position = glm::vec3(jlightPos[0], jlightPos[1], jlightPos[2]);
    glm::vec3 nonNormalizedColor = glm::vec3(jlightColor[0], jlightColor[1], jlightColor[2]);
    light.color = nonNormalizedColor / glm::vec3(255.0f);
    lights.push_back(light);
  }
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
    lightBatch->draw(lightProgramId, glslProcessor, lights);
    lightBatch->unbindBatch();
  }
  glUseProgram(0);

  glUseProgram(modelProgramId);
  {
    modelBatch->bindBatch();
    modelBatch->uploadCameraUniforms(modelProgramId, glslProcessor, camera3D);
    modelBatch->uploadLightUniforms(modelProgramId, glslProcessor, lights);
    modelBatch->draw(modelProgramId, glslProcessor, mesh);
    modelBatch->unbindBatch();
  }
  glUseProgram(0);

  window->swapBuffers();
}

int main(int argc, char const *argv[])
{

  if (argc < 2)
  {
    std::cerr << "Requires a Mesh path.\n";
    return 1;
  }

  const char* jsonFile = NULL;

  if (argc >= 3)
  {
    jsonFile = argv[2];
  }

  Window* window = new Window;
  Renderer3D* renderer3D = new Renderer3D(window);

  window->create(1024, 768, "Renderer3D");
  renderer3D->initialize(argv[1], jsonFile);

  window->run(&Renderer3D::update, &Renderer3D::render, *renderer3D);

  delete renderer3D;
  delete window;

  return 0;
}
