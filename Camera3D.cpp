#include "Camera3D.h"

void Camera3D::init(glm::vec3 position) {

  perspectiveMatrix = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 100.0f);

  lookDirection = glm::vec3(0.0f, 0.0f, 1.0f);
  eye           = position;
  center        = position + lookDirection;
  up            = glm::vec3(0.0f, 1.0f, 0.0f);

  viewMatrix = glm::lookAt(eye, center, up);
}

void Camera3D::translateXZPlane(float amount)
{
  glm::vec3 translation = lookDirection * amount;
  eye += translation;
  center +=translation;
}

void Camera3D::translateYAxis(float amount)
{
  glm::vec3 translation = glm::vec3(0.0f, amount, 0.0f);
  eye += translation;
  center += translation;
}

void Camera3D::rotate(float amount)
{
  glm::mat4 rotationMat = glm::rotate(glm::mat4(1), amount, up);
  lookDirection = glm::vec3(rotationMat * glm::vec4(lookDirection, 1.0f));
  center = eye + lookDirection;
}

void Camera3D::updateViewMatrix()
{
  viewMatrix = glm::lookAt(eye, center, up);
}

glm::mat4 Camera3D::getPerspectiveMatrix()
{
  return perspectiveMatrix;
}

glm::mat4 Camera3D::getViewMatrix()
{
  return viewMatrix;
}
