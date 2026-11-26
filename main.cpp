#include "src/Engine.h"
#include "src/OMP.h"

int main(int argc, char **argv) {
    Engine engine;
    OMP omp;
    engine.loadHeightMap("assets/models/Ridge Through Terrain Height Map.png");
    engine.run();
}