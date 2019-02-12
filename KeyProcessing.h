#ifndef KEY_PROCESSING_H
#define KEY_PROCESSING_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include "Window.h"
#include "Camera3D.h"

#define FORWARD    0x01
#define BACKWARDS  0x02
#define ROTATE_POS 0x04
#define ROTATE_NEG 0x08
#define MOVE_UP    0x10
#define MOVE_DOWN  0x20

class KeyProcessing
{
public:

  KeyProcessing(Window* window, Camera3D* camera3D);

  void invokeProcessing();

  static void externalKeyCallback(GLFWwindow* wnd,
                                  int         key,
                                  int         scanCode,
                                  int         action,
                                  int         mods);

  void internalKeyCallBack(GLFWwindow* wnd,
                           int         key,
                           int         scanCode,
                           int         action,
                           int         mods);

  void transformCamera();

private:

  unsigned int cameraFlags = 0x0;
  Window*      window;
  Camera3D*    camera3D;

  std::unordered_map<int, unsigned int> keyMappings;

};

#endif
