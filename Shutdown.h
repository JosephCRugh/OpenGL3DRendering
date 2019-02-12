#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include <functional>

/**
 * Shuts down the Window with a formatted
 * error message.
 */
void shutDown(const char* err, ...);

/**
 * Shuts down the Window and calls the callback.
 */
void shutDown(std::function<void()> postCallback);

#endif
