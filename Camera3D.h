#ifndef CAMERA_3D
#define CAMERA_3D

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera3D
{
public:

  /**
   * Initializes the perspective and view matrices
   * for rendering the 3D scene.
   */
  void init(glm::vec3 position);

  void updateViewMatrix();

  /**
   * Translates the camera in the direction of the
   * lookDirection on the X/Z plane.
   */
  void translateXZPlane(float amount);

  /**
   * Translates the camera upwards by increasing
   * the Y values of the eye and center.
   */
  void translateYAxis(float amount);

  /**
   * Rotates the camera's lookDirection by an
   * amount specified in radians.
   */
  void rotate(float amount);

  glm::mat4 getPerspectiveMatrix();

  glm::mat4 getViewMatrix();

private:

  /**
   * The position of where the camera is at.
   */
  glm::vec3 eye;

  /**
   * The point the camera is looking at.
   */
  glm::vec3 center;

  /**
   * The direction that is up with respect to
   * the camera.
   */
  glm::vec3 up;

  /**
   * A unit vector representing which direction
   * the camera is looking.
   */
  glm::vec3 lookDirection;

  float rotationRadians;

  glm::mat4 perspectiveMatrix;
  glm::mat4 viewMatrix;
};

#endif
