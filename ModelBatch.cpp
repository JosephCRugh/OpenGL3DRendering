#include "ModelBatch.h"
#include "BatchUtils.h"

ModelBatch::~ModelBatch()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbos[POSITION_ATTRIBUTE]);
  glDeleteBuffers(1, &vbos[NORMAL_ATTRIBUTE]);
  glDeleteBuffers(1, &ebo);
  delete[] vbos;
}

void ModelBatch::generateBuffers()
{
  glGenVertexArrays(1, &vao);

  vbos = new GLuint[2];
  glGenBuffers(1, &vbos[POSITION_ATTRIBUTE]);
  glGenBuffers(1, &vbos[NORMAL_ATTRIBUTE]);

  glGenBuffers(1, &ebo);
}

void ModelBatch::mapBuffersInformation()
{
  bindBatch();

  setupVertexBufferObject(POSITION_ATTRIBUTE, 3);
  setupVertexBufferObject(NORMAL_ATTRIBUTE, 3);

  unbindBatch();
}

void ModelBatch::setupVertexBufferObject(int attributeIndex, int dataLength)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbos[attributeIndex]);
  glEnableVertexAttribArray(attributeIndex);
  glVertexAttribPointer(attributeIndex, dataLength, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
}

void ModelBatch::uploadVertexBufferObjectData(int attributeIndex, std::vector<GLfloat> data)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbos[attributeIndex]);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), &data[0], GL_DYNAMIC_DRAW);
}

void ModelBatch::uploadCameraUniforms(GLuint programId, GlslProcessor* glslProcessor, Camera3D* camera3D)
{
  BatchUtils::uploadCameraUniforms(programId, glslProcessor, camera3D);
}

void ModelBatch::uploadLightUniforms(GLuint programId, GlslProcessor* glslProcessor, std::vector<Light> lights)
{
  glUniform1f(glslProcessor->getUniform(programId, "ambient"), 0.5f);

  for (int i = 0; i < lights.size(); i++)
  {
    std::string uniformLight = "lights[" + std::to_string(i) + "].";
    std::string uniformPosition = uniformLight + "position";
    std::string uniformColor = uniformLight + "color";
    glUniform3fv(glslProcessor->getUniform(programId, uniformPosition.c_str()), 1, glm::value_ptr(lights[i].position));
    glUniform3fv(glslProcessor->getUniform(programId, uniformColor.c_str()), 1, glm::value_ptr(lights[i].color));
  }
}

void ModelBatch::draw(GLuint programId, GlslProcessor* glslProcessor, Mesh* mesh)
{

  UploadData* uploadData = decomposeMesh(mesh);

  // Limiting the models to be 2x2x2.
  glm::mat4 shinkScaling = glm::scale(glm::mat4(1),
      glm::vec3(2.0f) / (mesh->highVertex - mesh->lowVertex));

  glm::mat4 meshScale = glm::scale(glm::mat4(1), mesh->scale);
  glm::mat4 meshTranslation = glm::translate(glm::mat4(1), mesh->position);

  glm::mat4 modelMatrix = shinkScaling * meshScale * meshTranslation;

  glUniformMatrix4fv(
    glslProcessor->getUniform(programId, "modelMatrix"),
    1,
    GL_FALSE,
    glm::value_ptr(modelMatrix));

  uploadVertexBufferObjectData(POSITION_ATTRIBUTE, uploadData->positionData);
  uploadVertexBufferObjectData(NORMAL_ATTRIBUTE, uploadData->normalData);

  std::vector<GLint> indicesData = uploadData->indicesData;
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size() * 4, &indicesData[0], GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(POSITION_ATTRIBUTE);
  glEnableVertexAttribArray(NORMAL_ATTRIBUTE);

  glDrawElements(GL_TRIANGLES, uploadData->indicesData.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(POSITION_ATTRIBUTE);
  glDisableVertexAttribArray(NORMAL_ATTRIBUTE);

  delete uploadData;
}

UploadData* ModelBatch::decomposeMesh(Mesh* mesh)
{
  UploadData* uploadData = new UploadData;
  uploadData->positionData.resize(mesh->vertexPositions.size() * 3);
  uploadData->normalData.resize(mesh->vertexNormals.size() * 3);
  uploadData->indicesData.resize(mesh->triangles.size() * 3);

  for (size_t i = 0; i < mesh->vertexPositions.size(); i++)
  {
    size_t os = i * 3;
    uploadData->positionData[os] = mesh->vertexPositions[i].x;
    uploadData->positionData[os + 1] = mesh->vertexPositions[i].y;
    uploadData->positionData[os + 2] = mesh->vertexPositions[i].z;
  }

  for (size_t i = 0; i < mesh->vertexNormals.size(); i++)
  {
    size_t os = i * 3;
    uploadData->normalData[os] = mesh->vertexNormals[i].x;
    uploadData->normalData[os + 1] = mesh->vertexNormals[i].y;
    uploadData->normalData[os + 2] = mesh->vertexNormals[i].z;
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

void ModelBatch::bindBatch()
{
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void ModelBatch::unbindBatch()
{
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
