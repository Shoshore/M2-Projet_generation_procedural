#include "Controls.h"
#include <iostream>
#include <GLFW/glfw3.h>

Controls::Controls(Viewport &viewport) : m_viewport(viewport) {
    m_camera.resize(viewport.width(), viewport.height());

    GLFWwindow *w = viewport.window();
    glfwSetWindowUserPointer(w, this);
    glfwSetKeyCallback(w, keyCallback);
    glfwSetMouseButtonCallback(w, mouseButtonCallback);
    glfwSetCursorPosCallback(w, cursorPosCallback);
    glfwSetScrollCallback(w, scrollCallback);
    glfwSetWindowSizeCallback(w, sizeCallback);
}

static Controls *self(GLFWwindow *w) {
    return static_cast<Controls *>(glfwGetWindowUserPointer(w));
}

void Controls::keyCallback(GLFWwindow *w, int key, int, int action, int) { self(w)->onKey(key, action); }
void Controls::mouseButtonCallback(GLFWwindow *w, int b, int a, int) { self(w)->onMouseButton(b, a); }
void Controls::cursorPosCallback(GLFWwindow *w, double x, double y) { self(w)->onCursorPos(x, y); }
void Controls::scrollCallback(GLFWwindow *w, double, double yoffset) { self(w)->onScroll(yoffset); }
void Controls::sizeCallback(GLFWwindow *w, int width, int height) { self(w)->onSize(width, height); }

void Controls::printUsage() {
    std::cerr << "\nCommandes\n------------------\n"
              << " ZQSD / WASD: deplacer la camera (combinables)\n"
              << " <molette>: zoom\n"
              << " <drag>+<clic gauche>: rotation\n"
              << " <drag>+<clic droit>: deplacement\n"
              << " ?: aide\n\n";
}

bool Controls::isDown(int key) const {
    return glfwGetKey(m_viewport.window(), key) == GLFW_PRESS;
}

void Controls::update() {
    float dx = 0.0f, dz = 0.0f;

    if (isDown(GLFW_KEY_W)) dz += m_speed;
    if (isDown(GLFW_KEY_S)) dz -= m_speed;
    if (isDown(GLFW_KEY_A)) dx += m_speed;
    if (isDown(GLFW_KEY_D)) dx -= m_speed;

    if (dx != 0.0f || dz != 0.0f)
        m_camera.move(dx, 0.0f, dz);
}


void Controls::onKey(int key, int action) {
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_SLASH || key == GLFW_KEY_H)
        printUsage();
    else if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(m_viewport.window(), GL_TRUE);
}

void Controls::onMouseButton(int button, int action) {
    m_pressed = (action != GLFW_RELEASE);
    m_move = m_rotate = false;

    if (action == GLFW_RELEASE) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        m_camera.beginRotate(m_lastX, m_lastY);
        m_rotate = true;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        m_move = true;
    }
}

void Controls::onCursorPos(double xpos, double ypos) {
    int x = (int)xpos, y = (int)ypos;

    if (!m_pressed) {
        m_lastX = x;
        m_lastY = y;
    }

    if (m_rotate) {
        m_camera.rotate(x, y);
    } else if (m_move) {
        m_camera.move((x - m_lastX) / static_cast<float>(m_viewport.width()),
                      (m_lastY - y) / static_cast<float>(m_viewport.height()), 0.0);
        m_lastX = x;
        m_lastY = y;
    }
}

void Controls::onScroll(double yoffset) {
    m_camera.zoom(-(float)yoffset * m_zoomSpeed);
}

void Controls::onSize(int width, int height) {
    m_viewport.resize(width, height);
    m_camera.resize(width, height);
}