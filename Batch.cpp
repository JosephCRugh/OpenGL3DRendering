#include "Batch.h"

Batch3D::~Batch3D()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbos[POSITION_ATTRIBUTE]);
  // glDeleteBuffers(1, &vbos[NORMAL_ATTRIBUTE]);
  glDeleteBuffers(1, &ebo);
  delete vbos;
}

void Batch3D::generateBuffers()
{
  glGenVertexArrays(1, &vao);

  vbos = new GLuint[2];
  glGenBuffers(1, &vbos[POSITION_ATTRIBUTE]);
  // glGenBuffers(1, &vbos[NORMAL_ATTRIBUTE]);

  glGenBuffers(1, &ebo);
}

void Batch3D::mapBuffersInformation()
{
  bindBuffer();

  setupVertexBufferObject(POSITION_ATTRIBUTE, 3);
  // setupVertexBufferObject(NORMAL_ATTRIBUTE, 3);

  unbindBuffer();
}

void Batch3D::setupVertexBufferObject(int attributeIndex, int dataLength)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbos[attributeIndex]);
  glEnableVertexAttribArray(attributeIndex);
  glVertexAttribPointer(attributeIndex, dataLength, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
}

void Batch3D::uploadVertexBufferObjectData(int attributeIndex, std::vector<GLfloat> data)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbos[attributeIndex]);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), &data[0], GL_DYNAMIC_DRAW);
}

void Batch3D::draw(GlslProcessor* glslProcessor, Camera3D* camera3D, GLuint programId, Mesh* mesh)
{
  bindBuffer();

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

  UploadData* uploadData = decomposeMesh(mesh);

  uploadVertexBufferObjectData(POSITION_ATTRIBUTE, uploadData->positionData);
  // uploadVertexBufferObjectData(NORMAL_ATTRIBUTE, uploadData->normalData);

  std::vector<GLint> indicesData = uploadData->indicesData;
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * 4, &indicesData[0], GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(POSITION_ATTRIBUTE);
  // glEnableVertexAttribArray(NORMAL_ATTRIBUTE);

  glDrawElements(GL_TRIANGLES, uploadData->indicesData.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(POSITION_ATTRIBUTE);
  // glDisableVertexAttribArray(NORMAL_ATTRIBUTE);

  delete uploadData;

  unbindBuffer();
}

UploadData* Batch3D::decomposeMesh(Mesh* mesh)
{
  UploadData* uploadData = new UploadData;
  uploadData->positionData.resize(mesh->vertexPositions.size() * 3);
  uploadData->normalData.resize(mesh->vertexNormals.size() * 3);
  uploadData->indicesData.resize(mesh->triangles.size() * 3);

  for (size_t i = 0; i < mesh->vertexPositions.size(); i++)
  {
    size_t os = i * 3;
    uploadData->positionData[os] = mesh->vertexPositions[i].x;
    uploadData->positionData[os+1] = mesh->vertexPositions[i].y;
    uploadData->positionData[os+2] = mesh->vertexPositions[i].z;
  }

  for (size_t i = 0; i < mesh->vertexNormals.size(); i++)
  {
    size_t os = i * 3;
    uploadData->normalData[os] = mesh->vertexNormals[i].x;
    uploadData->normalData[os+1] = mesh->vertexNormals[i].y;
    uploadData->normalData[os+2] = mesh->vertexNormals[i].z;
  }

  for (size_t i = 0; i < mesh->triangles.size(); i++)
  {
    size_t os = i * 3;
    uploadData->indicesData[os] = mesh->triangles[i][0];
    uploadData->indicesData[os+1] = mesh->triangles[i][1];
    uploadData->indicesData[os+2] = mesh->triangles[i][2];
  }

  return uploadData;
}

void Batch3D::bindBuffer()
{
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Batch3D::unbindBuffer()
{
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
