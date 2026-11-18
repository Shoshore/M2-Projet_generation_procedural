// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include "src/OpenGL.h"
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "src/Camera.h"
#include "src/Mesh.h"
#include <algorithm>

#include <glm/glm.hpp>

using namespace std;

// window parameters
GLFWwindow *g_window = NULL;
int g_windowWidth = 1024;
int g_windowHeight = 768;

static Camera camera;
const float camera_speed = 0.2;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static bool mouseIsPressed = false;
static int lastX = 0, lastY = 0, lastZoom = 0;
static unsigned int FPS = 0;
static bool fullScreen = false;

Mesh mesh;

glm::vec3 getRightVector() {
    float modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    return glm::vec3(modelview[0], modelview[4], modelview[8]);
}
glm::vec3 getUpVector() {
    float modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    return glm::vec3(modelview[1], modelview[5], modelview[9]);
}
glm::vec3 getViewVector() {
    float modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    return glm::vec3(modelview[2], modelview[6], modelview[10]);
}

void printUsage() {
    cerr << endl
         << "Usage : ./gmini [<file.off>]" << endl
         << "Keyboard commands" << endl
         << "------------------" << endl
         << " ?: Print help" << endl
         << " f: Toggle full screen mode" << endl
         << " <drag>+<left button>: rotate model" << endl
         << " <drag>+<right button>: move model" << endl
         << " <drag>+<middle button>: zoom" << endl
         << endl;
}

void usage() {
    printUsage();
    exit(EXIT_FAILURE);
}

void initLight() {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f, -16.0f, -50.0f};
    GLfloat color1[4] = {1, 1, 1, 1};
    GLfloat ambient[4] = {1, 1, 1, 1};

    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

void init() {
    camera.resize(g_windowWidth, g_windowHeight);
    initLight();
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
}

void render() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, g_windowWidth, g_windowHeight);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.apply();

    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glColor3f(0.4, 0.4, 0.8);
    mesh.draw();
}

// Executed each time a key is entered.
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
    if (action == GLFW_RELEASE)
        return;

    switch (key) {
        case GLFW_KEY_W:
            camera.move(0.0, 0.0, camera_speed);
            break;
        case GLFW_KEY_S:
            camera.move(0.0, 0.0, -camera_speed);
            break;
        case GLFW_KEY_A:
            camera.move(camera_speed, 0.0, 0.0);
            break;
        case GLFW_KEY_D:
            camera.move(-camera_speed, 0.0, 0.0);
            break;

    default:
        printUsage();
        break;
    }
}

// Called each time a mouse button is pressed
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mods) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    int x = lastX;
    int y = lastY;

    mouseIsPressed = state != GLFW_RELEASE;

    // moving the camera:
    if (state == GLFW_RELEASE) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            camera.beginRotate(x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
}

// Called each time the mouse cursor moves
void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    int x = xpos;
    int y = ypos;

    if (!mouseIsPressed) {
        lastX = x;
        lastY = y;
    }

    // moving the camera:
    if (mouseRotatePressed == true) {
        camera.rotate(x, y);
    } else if (mouseMovePressed == true) {
        camera.move((x - lastX) / static_cast<float>(g_windowWidth),
                    (lastY - y) / static_cast<float>(g_windowHeight), 0.0);
        lastX = x;
        lastY = y;
    } else if (mouseZoomPressed == true) {
        camera.zoom(float(y - lastZoom) / g_windowHeight);
        lastZoom = y;
    }
}

// Executed each time the window is resized. Adjust the aspect ratio and the
// rendering viewport to the current window.
void windowSizeCallback(GLFWwindow *window, int width, int height) {
    g_windowWidth = width;
    g_windowHeight = height;
    camera.resize(width, height);
    glViewport(
        0, 0, (GLint)width,
        (GLint)height); // Dimension of the rendering region in the window
}

void initGLFW() {
    // Initialize GLFW, the library responsible for window management
    if (!glfwInit()) {
        std::cerr << "ERROR: Failed to init GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Before creating the window, set some option flags
    // WE DON'T CARE ABOUT MODERN OPENGL, WE'RE DOING GEOMETRY HERE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create the window
    g_window = glfwCreateWindow(g_windowWidth, g_windowHeight,
                                "IG3DA - Practical", NULL, NULL);
    if (!g_window) {
        std::cerr << "ERROR: Failed to open window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    // Load the OpenGL context in the GLFW window using GLAD OpenGL wrangler
    glfwMakeContextCurrent(g_window);

    // not mandatory for all, but MacOS X
    glfwGetFramebufferSize(g_window, &g_windowWidth, &g_windowHeight);

    // Connect the callbacks for interactive control
    glfwSetWindowSizeCallback(g_window, windowSizeCallback);
    glfwSetKeyCallback(g_window, keyCallback);
    glfwSetCursorPosCallback(g_window, cursorPosCallback);
    glfwSetMouseButtonCallback(g_window, mouseButtonCallback);
}

void exitOnCriticalError(const std::string &message) {
    std::cerr << "> [Critical error]" << message << std::endl;
    std::cerr << "> [Exit]" << std::endl;
    std::exit(EXIT_FAILURE);
}

void initOpenGL() {
    // Load extensions for modern OpenGL
    //    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    //        exitOnCriticalError("[Failed to initialize OpenGL context]");

    if (!gladLoadGL())
        exitOnCriticalError("[Failed to initialize OpenGL context]");

    // glEnable(GL_DEBUG_OUTPUT);    // Modern error callback functionality
    // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // For recovering the line where
    // the error occurs, set a debugger breakpoint in DebugMessageCallback
    // glDebugMessageCallback(debugMessageCallback, 0); // Specifies the
    // function to call when an error message is generated.

    glCullFace(GL_BACK); // Specifies the faces to cull (here the ones pointing
                         // away from the camera)
    glEnable(GL_CULL_FACE);  // Enables face culling (based on the orientation
                             // defined by the CW/CCW enumeration).
    glDepthFunc(GL_LESS);    // Specify the depth test for the z-buffer
    glEnable(GL_DEPTH_TEST); // Enable the z-buffer test in the rasterization
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // specify the background color, used
                                          // any time the framebuffer is cleared
}

int main(int argc, char **argv) {
    initGLFW();   // Windowing system
    initOpenGL(); // OpenGL Context and shader pipeline
    init();

    mesh.loadOFF(argc == 2 ? argv[1] : "models/arma.off");

    while (!glfwWindowShouldClose(g_window)) {
        // update(static_cast<float>(glfwGetTime()));
        render();
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
    //  clear();
    std::cout << " > Quit" << std::endl;
    return EXIT_SUCCESS;
}
