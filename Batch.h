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

class Batch3D
{
public:

  ~Batch3D();

  /**
   * Generates the vertex array, and vertex buffer
   * objects for rendering.
   */
  void generateBuffers();

  /**
   * Maps the information on the CPU for the
   * attributes to the attributes on the GPU.
   */
  void mapBuffersInformation();

  /**
   * Renders the 3D image of the scene.
   */
  void draw(GlslProcessor* glslProcessor, Camera3D* camera3D, GLuint programId, Mesh* mesh);

private:

  void setupVertexBufferObject(int attributeIndex, int dataLength);

  void uploadVertexBufferObjectData(int attributeIndex, std::vector<GLfloat> data);

  UploadData* decomposeMesh(Mesh* mesh);

  /**
   * Binds the vertex array, and vertex buffer objects.
   */
  void bindBuffer();

  /**
   * Unbinds the vertex array, and vertex buffer objects.
   */
  void unbindBuffer();

  GLuint  vao;
  GLuint* vbos;
  GLuint  ebo;
};

#endif
