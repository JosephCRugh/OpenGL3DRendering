#include "KeyProcessing.h"

KeyProcessing::KeyProcessing(Window* window, Camera3D* camera3D)
: window(window)
, camera3D(camera3D)
{
  keyMappings =
  {
      { GLFW_KEY_W, FORWARD    },
      { GLFW_KEY_S, BACKWARDS  },
      { GLFW_KEY_D, ROTATE_POS },
      { GLFW_KEY_A, ROTATE_NEG },
      { GLFW_KEY_Q, MOVE_UP    },
      { GLFW_KEY_E, MOVE_DOWN  }
  };

}

void KeyProcessing::externalKeyCallback(GLFWwindow* wnd,
                                        int         key,
                                        int         scanCode,
                                        int         action,
                                        int         mods)
{
  KeyProcessing* inputProcesing = static_cast<KeyProcessing*>(glfwGetWindowUserPointer(wnd));
  inputProcesing->internalKeyCallBack(wnd, key, scanCode, action, mods);
}

void KeyProcessing::internalKeyCallBack(GLFWwindow* wnd,
                                        int         key,
                                        int         scanCode,
                                        int         action,
                                        int         mods)
{
  switch (action) {
    case GLFW_PRESS:

      if (keyMappings.find(key) != keyMappings.end())
      {
        this->cameraFlags |= keyMappings[key];
      }

      break;
    case GLFW_RELEASE:

      if (keyMappings.find(key) != keyMappings.end())
      {
        this->cameraFlags &= ~keyMappings[key];
      }

      break;
    }
}

void KeyProcessing::invokeProcessing()
{
  glfwSetWindowUserPointer(window->getGlfwWindow(), this);
  glfwSetKeyCallback(window->getGlfwWindow(), externalKeyCallback);
}

void KeyProcessing::transformCamera()
{

  if ((cameraFlags & BACKWARDS) != 0)
  {
    this->camera3D->translateXZPlane(-0.2f);
  }

  if ((cameraFlags & FORWARD) != 0)
  {
    this->camera3D->translateXZPlane(+0.2f);
  }

  if ((cameraFlags & ROTATE_POS) != 0)
  {
    this->camera3D->rotate(-0.015f);
  }

  if ((cameraFlags & ROTATE_NEG) != 0)
  {
    this->camera3D->rotate(+0.015f);
  }

  if ((cameraFlags & MOVE_UP) != 0)
  {
    this->camera3D->translateYAxis(+0.15f);
  }

  if ((cameraFlags & MOVE_DOWN) != 0)
  {
    this->camera3D->translateYAxis(-0.15f);
  }
}
