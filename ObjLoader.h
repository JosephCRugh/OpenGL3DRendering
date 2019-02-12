#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <unordered_map>
#include "Geometry.h"

class ObjLoader
{
private:

  struct IndiceTuple
  {
    IndiceTuple() {}

    IndiceTuple(int posIndex, int normIndex)
    : posIndex(posIndex)
    , normIndex(normIndex) {}
    int posIndex;
    int normIndex;
    int uniqueIndex;
  };

public:

  /**
   * Converts .obj files generated in blender into a mesh.
   * Since the .obj files come as multi-indice based mapping
   * it must be turned into single indice based mapping so
   * that OpenGL may understand it.
   */
  Mesh* loadMesh(const char* file);

private:

  /**
   * Creates a unique tuple associated with normals/positions.
   * This is used so that the tuple may be referenced later and
   * determine the ordering of the vertex data.
   */
  void generateUniqueTuple(int                      vertexIndex,
                           std::vector<std::string> vertexData,
                           Mesh*                    mesh,
                           std::vector<glm::vec3>   positions,
                           std::vector<glm::vec3>   normals,
                           int                      indiceIndex);

  /**
   * Generates a new tuple index.
   */
  void generateNewIndex(IndiceTuple&           indicieTuple,
                        std::vector<glm::vec3> positions,
                        std::vector<glm::vec3> normals);

  std::vector<std::string> splitString(std::string str, char ch);

  /**
   * The current index of the triangle being
   * worked with.
   */
  int triangleIndex    = 0;

  /**
  * The current index of generated unique tuples.
  */
  int uniqueIndexCount = 0;

  /**
   * Position -> Normal -> IndiceTuple.
   */
  using InnerMap = std::unordered_map<int, IndiceTuple>;
  std::unordered_map<int, InnerMap> indicesMapping;

  std::vector<glm::vec3> uniquePositions;
  std::vector<glm::vec3> uniqueNormals;

};

#endif
