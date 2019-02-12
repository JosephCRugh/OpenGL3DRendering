#include "ObjLoader.h"
#include "Shutdown.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
#include <limits>

using namespace std;

Mesh* ObjLoader::loadMesh(const char* file)
{

  Mesh* mesh = new Mesh;

  ifstream inStream(file);

  if (!inStream)
  {
    shutDown([&file](){ perror(file); });
  }

  vector<glm::vec3> positions;
  vector<glm::vec3> normals;

  // Reading in the triangle positional and normal
  // information.
  string line;
  int lineCount = 0;
  int faceStartIndex = -1;
  bool foundFaceStart = false;
  vector<string> lines;
  mesh->lowVertex = glm::vec3(numeric_limits<float>::max());
  mesh->highVertex = glm::vec3(std::numeric_limits<float>::min());
  while (getline(inStream, line))
  {
    if (line.rfind("v ", 0) == 0)
    {
      vector<string> parts = splitString(line, ' ');
      glm::vec3 position = glm::vec3(stof(parts[1]), stof(parts[2]), stof(parts[3]));
      positions.push_back(position);
      mesh->lowVertex.x = fmin(position.x, mesh->lowVertex.x);
      mesh->lowVertex.y = fmin(position.y, mesh->lowVertex.y);
      mesh->lowVertex.z = fmin(position.z, mesh->lowVertex.z);
      mesh->highVertex.x = fmax(position.x, mesh->highVertex.x);
      mesh->highVertex.y = fmax(position.y, mesh->highVertex.y);
      mesh->highVertex.z = fmax(position.z, mesh->highVertex.z);
    }
    else if (line.rfind("vn ", 0) == 0)
    {
      vector<string> parts = splitString(line, ' ');
      glm::vec3 normal = glm::vec3(stof(parts[1]), stof(parts[2]), stof(parts[3]));
      normals.push_back(normal);
    }
    else if (line.rfind("f ", 0) == 0)
    {
      if (!foundFaceStart)
      {
        faceStartIndex = lineCount;
        foundFaceStart = true;
      }
    }
    lines.push_back(line);
    lineCount++;
  }

  mesh->triangles.resize(lineCount - faceStartIndex);

  // Since there are 3 vertices per triangle it's *3, one
  // indice piece of information per triangle.
  // indicesOrdering = new int[mesh->triangles.size() * 3];

  for (int i = faceStartIndex; i < lines.size(); i++)
  {
    vector<string> vertexInfo = splitString(lines[i], ' ');
    vector<string> v1 = splitString(vertexInfo[1], '/');
    vector<string> v2 = splitString(vertexInfo[2], '/');
    vector<string> v3 = splitString(vertexInfo[3], '/');
    int indiceIndex = (i - faceStartIndex);

    // There are 3 vertices per triangle.
    mesh->triangles[triangleIndex] = new int[3];

    generateUniqueTuple(0, v1, mesh, positions, normals, indiceIndex);
    generateUniqueTuple(1, v2, mesh, positions, normals, indiceIndex);
    generateUniqueTuple(2, v3, mesh, positions, normals, indiceIndex);
    triangleIndex++;
  }

  // There are as many positions as there are unique positions.
  // Same goes for normals but there is always a matching size
  // between the two.
  mesh->vertexPositions.resize(uniquePositions.size());
  mesh->vertexNormals.resize(uniquePositions.size());

  for (size_t i = 0; i < uniquePositions.size(); i++)
  {
    mesh->vertexPositions[i] = uniquePositions[i];
    mesh->vertexNormals[i] = uniqueNormals[i];
  }

  // Cleaning up data by resetting it.
  triangleIndex = 0;
  uniqueIndexCount = 0;
  indicesMapping.clear();
  uniquePositions.clear();
  uniqueNormals.clear();

  return mesh;
}

void ObjLoader::generateUniqueTuple(int               vertexIndex,
                                    vector<string>    vertexData,
                                    Mesh*             mesh,
                                    vector<glm::vec3> positions,
                                    vector<glm::vec3> normals,
                                    int               indiceIndex)
{
  int posIndex  = stoi(vertexData[0]) - 1;
  int normIndex = stoi(vertexData[2]) - 1;

  IndiceTuple indiceTuple(posIndex, normIndex);

  Triangle triangle = mesh->triangles[triangleIndex];

  if (indicesMapping.find(posIndex) != indicesMapping.end())
  {
    InnerMap innerMap = indicesMapping[posIndex];
    if (innerMap.find(normIndex) != innerMap.end())
    {
      indiceTuple.uniqueIndex = innerMap[normIndex].uniqueIndex;
    }
    else
    {
      generateNewIndex(indiceTuple, positions, normals);
      innerMap.insert(pair<int, IndiceTuple>(normIndex, indiceTuple));
    }
  }
  else
  {
    generateNewIndex(indiceTuple, positions, normals);
    indicesMapping[posIndex] = { { normIndex, indiceTuple } };
  }

  triangle[vertexIndex] = indiceTuple.uniqueIndex;

}

void ObjLoader::generateNewIndex(IndiceTuple&      indiceTuple,
                                 vector<glm::vec3> positions,
                                 vector<glm::vec3> normals)
{
  indiceTuple.uniqueIndex = uniqueIndexCount++;
  uniquePositions.push_back(positions[indiceTuple.posIndex]);
  uniqueNormals.push_back(normals[indiceTuple.normIndex]);
}

vector<string> ObjLoader::splitString(string str, char ch)
{
  stringstream ss(str);

  vector<string> parts;
  string part;
  while (std::getline(ss, part, ch))
  {
    parts.push_back(part);
  }
  return parts;
}
