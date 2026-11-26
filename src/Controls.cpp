#include "Controls.h"
#include <GLFW/glfw3.h>

static Controls *self(GLFWwindow *w) {
    return static_cast<Controls *>(glfwGetWindowUserPointer(w));
}


Controls::Controls(Viewport &viewport) : m_viewport(viewport) {
    m_camera.resize(viewport.width(), viewport.height());

    GLFWwindow *w = viewport.window();
    glfwSetWindowUserPointer(w, this);
    glfwSetKeyCallback(w, keyCallback);
    glfwSetCursorPosCallback(w, cursorPosCallback);
    glfwSetFramebufferSizeCallback(w, sizeCallback);
    glfwSetMouseButtonCallback(w, mouseButtonCallback);

    setCursorCaptured(true);
}


void Controls::keyCallback(GLFWwindow *w, int key, int, int action, int) { self(w)->onKey(key, action); }
void Controls::cursorPosCallback(GLFWwindow *w, double x, double y) { self(w)->onCursorPos(x, y); }
void Controls::sizeCallback(GLFWwindow *w, int width, int height) { self(w)->onSize(width, height); }

void Controls::mouseButtonCallback(GLFWwindow *w, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        self(w)->setCursorCaptured(true);
}


bool Controls::isDown(int key) const {
    return glfwGetKey(m_viewport.window(), key) == GLFW_PRESS;
}


void Controls::update() {
    if (isDown(GLFW_KEY_W)) m_camera.moveForward();
    if (isDown(GLFW_KEY_S)) m_camera.moveBackward();
    if (isDown(GLFW_KEY_A)) m_camera.moveLeft();
    if (isDown(GLFW_KEY_D)) m_camera.moveRight();
    if (isDown(GLFW_KEY_SPACE)) m_camera.moveUp();
    if (isDown(GLFW_KEY_LEFT_SHIFT)) m_camera.moveDown();
}


void Controls::setCursorCaptured(bool captured) {
    m_cursorCaptured = captured;
    glfwSetInputMode(m_viewport.window(), GLFW_CURSOR,
                     captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    m_firstMouse = true;
}


void Controls::onKey(int key, int action) {
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(m_viewport.window(), GL_TRUE);
    else if (key == GLFW_KEY_TAB)
        setCursorCaptured(!m_cursorCaptured);
    else if (key == GLFW_KEY_Z)
        m_wireframe = !m_wireframe;
}


void Controls::onCursorPos(double xpos, double ypos) {
    if (!m_cursorCaptured) return;

    if (m_firstMouse) {
        m_lastX = xpos; m_lastY = ypos;
        m_firstMouse = false;
    }
    m_camera.rotate(float(xpos - m_lastX), float(ypos - m_lastY));
    m_lastX = xpos; m_lastY = ypos;
}


void Controls::onSize(int width, int height) {
    if (width <= 0 || height <= 0) return;
    m_viewport.resize(width, height);
    m_camera.resize(width, height);
}