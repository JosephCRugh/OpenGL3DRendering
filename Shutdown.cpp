#include "Shutdown.h"
#include <cstdarg>

void shutDown(const char* err, ...)
{
  va_list args;
  va_start(args, err);
  vfprintf(stderr, err, args);
  va_end(args);
  exit(1);
}

void shutDown(std::function<void()> postCallback)
{
  postCallback();
  shutDown("");
}
