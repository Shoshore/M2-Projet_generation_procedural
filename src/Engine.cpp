#include "Engine.h"

Engine::Engine() {
    initGL();
}

void Engine::run() {
    while (!m_viewport.shouldClose()) {
        m_viewport.pollEvents();
        m_controls.update();
        render();
        m_viewport.swapBuffers();
    }
}


void Engine::initGL() {
    initLight();
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
}


void Engine::initLight() {
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

void Engine::loadMesh(const char *path) {
    m_mesh.loadOFF(path);
}

void Engine::render() {
    glViewport(0, 0, m_viewport.width(), m_viewport.height());
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_controls.camera().apply();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glColor3f(0.4f, 0.4f, 0.8f);
    m_mesh.draw();
}