#ifndef PATCH_H
#define PATCH_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <lib/stb_image.h>

struct HeightMap {
    int width = 0, height = 0, nChannels = 0;
    unsigned char *image = nullptr;

    HeightMap(const std::string &filename) {
        image = stbi_load(filename.c_str(), &width, &height, &nChannels, 0);
    }

    ~HeightMap() {
        if (image)
            stbi_image_free(image);
    }

    float at(int x, int y) const {
        x = std::clamp(x, 0, width - 1);
        y = std::clamp(y, 0, height - 1);
        return image[(y * width + x) * nChannels];
    }
};

struct Patch {
    int cx, cy;
    float zi;
    std::vector<float> val;
};

struct Patch_extraction {
    int R = 30;
    int step = 30;
    int side() const { return 2 * R + 1; }

    std::vector<Patch> extraction_patches(const HeightMap &T) const {
        std::vector<Patch> patches;

        patches.reserve(size_t(T.width / step + 1) * (T.height / step + 1));

        for (int cy = 0; cy < T.height; cy += step)
            for (int cx = 0; cx < T.width; cx += step)
                patches.push_back(extract(T, cx, cy));
        return patches;
    }

    Patch extract(const HeightMap &T, int cx, int cy) const {
        Patch p;
        p.cx = cx;
        p.cy = cy;
        p.val.resize(side() * side());

        double sum = 0;
        int count = 0;
        for (int dy = -R; dy <= R; ++dy)
            for (int dx = -R; dx <= R; ++dx) {
                if (dx * dx + dy * dy <= R * R) {
                    sum += T.at(cx + dx, cy + dy);
                    ++count;
                }
            }
        p.zi = float(sum / count);

        for (int dy = -R; dy <= R; ++dy)
            for (int dx = -R; dx <= R; ++dx)
                p.val[(dy + R) * side() + (dx + R)] = T.at(cx + dx, cy + dy);

        return p;
    }
};

#endif
