#ifndef ENGINE
#define ENGINE

#include "Mesh.h"
#include "Viewport.h"
#include "Controls.h"

class Engine {
    public:
        Engine();
        void loadMesh(const char *path);
        void run();

    private:
        void initLight();
        void initGL();
        void render();

        Viewport m_viewport;
        Controls m_controls{m_viewport};
        Mesh m_mesh;
};

#endif