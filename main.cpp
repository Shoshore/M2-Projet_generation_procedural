#include "src/Engine.h"
#include "src/OMP.h"
int main(int argc, char **argv) {
    Engine engine;
    OMP omp;
    engine.loadMesh("assets/models/arma.off");
    engine.run();
}