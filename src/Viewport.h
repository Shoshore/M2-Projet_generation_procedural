#ifndef VIEWPORT
#define VIEWPORT

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Viewport {
public:
    Viewport(int width = 1024, int height = 768);
    ~Viewport();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    void resize(int width, int height);

    GLFWwindow *window() const { return m_window; }
    int width() const { return m_width; }
    int height() const { return m_height; }

private:
    GLFWwindow *m_window = nullptr;
    int m_width, m_height;
};

#endif