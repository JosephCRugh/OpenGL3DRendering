#ifndef BATCH_H
#define BATCH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "GlslProcessor.h"
#include "Camera3D.h"
#include "Geometry.h"

#define POSITION_ATTRIBUTE 0
#define NORMAL_ATTRIBUTE   1

struct UploadData
{
  std::vector<GLfloat> positionData;
  std::vector<GLfloat> normalData;
  std::vector<GLint> indicesData;
};

class ModelBatch
{
public:

  ~ModelBatch();

  /**
   * Generates the vertex array, elements buffer,
   * and vertex buffers objects for rendering.
   */
  void generateBuffers();

  /**
   * Maps the information on the CPU for the
   * attributes to the attributes on the GPU.
   */
  void mapBuffersInformation();

  /**
   * Binds the element array, and vertex array objects.
   */
  void bindBatch();

  /**
   * Unbinds the element array, and vertex array objects.
   */
  void unbindBatch();

  /**
   * Uploads the camera's view and project matrices.
   */
  void uploadCameraUniforms(GLuint programId, GlslProcessor* glslProcessor, Camera3D* camera3D);

  /**
   * Uploads the uniform light and positional values to apply
   * lighting effects on the mesh.
   */
  void uploadLightUniforms(GLuint programId, GlslProcessor* glslProcessor, std::vector<Light> lights);

  /**
   * Renders the 3D image of the scene.
   */
  void draw(GLuint programId, GlslProcessor* glslProcessor, Mesh* mesh);

private:

  void setupVertexBufferObject(int attributeIndex, int dataLength);

  void uploadVertexBufferObjectData(int attributeIndex, std::vector<GLfloat> data);

  UploadData* decomposeMesh(Mesh* mesh);

  GLuint  vao;
  GLuint* vbos;
  GLuint  ebo;
};

#endif
