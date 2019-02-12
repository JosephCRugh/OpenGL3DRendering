#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:

  ~Window();

  /**
   * Generates an instance of a GLFW window.
   */
  void create(int width, int height, const char* title);

  void setWindowIcon(const char* iconPath);

  /**
   * Swaps the frame buffer of the GLFW window.
   */
  void swapBuffers();

  /**
   * Continues to run until the client decides to
   * close the window.
   */
  template<typename T, class R>
  void run(T&& update, T&& render, R& bound)
  {
    while (!glfwWindowShouldClose(this->glfwWindow))
    {
      (bound.*update)( );
      (bound.*render)( );
    }
  }

  GLFWwindow* getGlfwWindow();

private:
  GLFWwindow* glfwWindow;
};

#endif
