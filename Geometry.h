#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <vector>

/**
 * Stores the indices which reference the
 * data for the triangle.
 */
using Triangle = int*;

struct Mesh
{
  ~Mesh()
  {
      for (Triangle triangle : triangles)
      {
        delete[] triangle;
      }
  }

  std::vector<Triangle> triangles;

  std::vector<glm::vec3> vertexPositions;
  std::vector<glm::vec3> vertexNormals;

};

#endif