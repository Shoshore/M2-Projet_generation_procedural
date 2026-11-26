#include "src/Engine.h"
#include "src/OMP.h"

int main(int argc, char **argv) {
    Engine engine;
    OMP omp;
    engine.loadHeightMap("assets/heightmap/iceland_heightmap.png");
    engine.run();
}