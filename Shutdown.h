#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include <functional>

void shutDown(const char* err, ...);

void shutDown(std::function<void()> postCallback);

#endif
