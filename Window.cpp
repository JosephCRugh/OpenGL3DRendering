#include "Window.h"

Window::~Window( )
{
  glfwDestroyWindow(this->glfwWindow);
  glfwTerminate();
}

void glfwErrorCallback(int error, const char* description)
{
  fprintf(stderr, "GLFW Error: %s\n", description);
}

void Window::create(int width, int height, const char* title)
{
  glfwSetErrorCallback(glfwErrorCallback);

  if (!glfwInit()) exit(1);

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  glfwWindow = glfwCreateWindow(
                              width,
                              height,
                              title,
                              NULL,
                              NULL);
  if (!glfwWindow)
  {
    fprintf(stderr, "Window was NULL.\n");
    glfwTerminate();
    exit(1);
  }

  glfwMakeContextCurrent(glfwWindow);
  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Failed To Initialize GLEW\n");
    glfwTerminate();
    exit(1);
  }
}

void Window::swapBuffers()
{
  glfwSwapBuffers(this->glfwWindow);
}

GLFWwindow* Window::getGlfwWindow()
{
  return glfwWindow;
}
