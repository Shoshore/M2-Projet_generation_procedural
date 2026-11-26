#ifndef ENGINE
#define ENGINE

#include "Controls.h"
#include "Mesh.h"
#include "Viewport.h"

class Engine {
  public:
    Engine();
    void loadMesh(const char *path);
    void run();
    void loadHeightMap(const char *path);

  private:
    void initLight();
    void initGL();
    void render();

    Viewport m_viewport;
    Controls m_controls{m_viewport};
    Mesh m_mesh;
};

#endif