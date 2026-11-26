#ifndef CONTROLS
#define CONTROLS

#include "Camera.h"
#include "Viewport.h"

class Controls {
  public:
    Controls(Viewport &viewport);
    Camera &camera() { return m_camera; }
    bool wireframe() const { return m_wireframe; }
    void update();

  private:

    static void keyCallback(GLFWwindow *w, int key, int scancode, int action,
                            int mods);
    static void cursorPosCallback(GLFWwindow *w, double x, double y);
    static void sizeCallback(GLFWwindow *w, int width, int height);
    static void mouseButtonCallback(GLFWwindow *w, int button, int action,
                                    int mods);

    void onKey(int key, int action);
    void onCursorPos(double x, double y);
    void onSize(int width, int height);

    void setCursorCaptured(bool captured);

    bool isDown(int key) const;

    Viewport &m_viewport;
    Camera m_camera;

    double m_lastX = 0.0, m_lastY = 0.0;
    bool m_firstMouse = true;
    bool m_wireframe = false;
    bool m_cursorCaptured = true;

};

#endif