#ifndef CONTROLS
#define CONTROLS

#include "Camera.h"
#include "Viewport.h"

class Controls {
    public:
        Controls(Viewport &viewport);
        Camera &camera() { return m_camera; }

        void update();

    private:
        static void keyCallback(GLFWwindow *w, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow *w, double x, double y);
        static void scrollCallback(GLFWwindow *w, double xoffset, double yoffset);
        static void sizeCallback(GLFWwindow *w, int width, int height);

        void onKey(int key, int action);
        void onMouseButton(int button, int action);
        void onCursorPos(double x, double y);
        void onScroll(double yoffset);
        void onSize(int width, int height);

        bool isDown(int key) const;
        static void printUsage();

        Viewport &m_viewport;
        Camera m_camera;
        const float m_speed = 0.1f;
        const float m_zoomSpeed = 1.0f;

        bool m_rotate = false, m_move = false, m_pressed = false;
        int m_lastX = 0, m_lastY = 0;
};

#endif