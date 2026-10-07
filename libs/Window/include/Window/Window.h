#pragma once

#include <string>

struct GLFWwindow;

class Window {
public:
    Window(int width, int height, const char* title);
   ~Window();

    void waitEvents()  const;
    void swapBuffers() const;
    void clear(float r = 0.39f, float g = 0.58f, float b = 0.93f, float a = 1.f) const;

    bool shouldClose() const;

private:
    GLFWwindow* m_window = nullptr;
};
