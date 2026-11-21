#include "src/Engine.h"

int main(int argc, char **argv) {
    Engine engine;
    engine.loadMesh("assets/models/arma.off");
    engine.run();
}