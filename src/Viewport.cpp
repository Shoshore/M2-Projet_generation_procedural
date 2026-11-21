#include "Viewport.h"
#include <cstdlib>
#include <iostream>

Viewport::Viewport(int width, int height) : m_width(width), m_height(height) {
    if (!glfwInit()) {
        std::cerr << "ERROR: Failed to init GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, "Project", NULL, NULL);
    if (!m_window) {
        std::cerr << "ERROR: Failed to open window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);
    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    if (!gladLoadGL()) {
        std::cerr << "ERROR: Failed to initialize OpenGL context" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}


Viewport::~Viewport() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}


bool Viewport::shouldClose() const { return glfwWindowShouldClose(m_window); }
void Viewport::swapBuffers() { glfwSwapBuffers(m_window); }
void Viewport::pollEvents() { glfwPollEvents(); }


void Viewport::resize(int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
}