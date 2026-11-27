#include "src/Engine.h"
#include "src/OMP.h"

#include "src/Patch.h"
#include <iostream>

int main(int argc, char **argv) {
    Engine engine;
    OMP omp;
    // engine.loadHeightMap("assets/heightmap/everest.png");
    engine.run();

    const HeightMap test = HeightMap("assets/heightmap/everest.png");
    std::cout << "valeur " << test.width << " " << test.height << std::endl;

    Patch_extraction extraction;
    std::vector<Patch> patches = extraction.extraction_patches(test);

    std::cout << "nb de patches : " << patches.size() << std::endl;

    for (Patch &p : patches) {
        std::cout << "coordonnées du centre, cx = " << p.cx << " cy = " << p.cy
                  << std::endl;

        std::cout << "zi = " << p.zi << std::endl;

        std::cout << "Taille du patch : " << p.val.size() << std::endl;

        // for (float v : p.val) {
        //     std::cout << " val = " << v << std::endl;
        // }
    }

    std::cout << "Done" << std::endl;
}