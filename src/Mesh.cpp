#include "Mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include <lib/stb_image.h>

#include <fstream>
#include <memory>
#include <stdexcept>

void Mesh::loadOFF(const std::string &filename) {
    std::ifstream in(filename.c_str());

    std::string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    V.resize(sizeV);
    T.resize(sizeT);
    for (unsigned int i = 0; i < sizeV; i++) {
        in >> V[i].p.x() >> V[i].p.y() >> V[i].p.z();
        V[i].pInit = V[i].p;
    }
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++)
            in >> T[i].v[j];
    }
    in.close();
    centerAndScaleToUnit();
    recomputeNormals();
}

void Mesh::loadHeightMap(const std::string &filename) {
    int width = 0;
    int height = 0;
    int nChannels = 0;
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> data(
        stbi_load(filename.c_str(), &width, &height, &nChannels, 0),
        stbi_image_free);

    V.clear();
    T.clear();
    V.reserve(static_cast<std::size_t>(width) * height);
    T.reserve(static_cast<std::size_t>(width - 1) * (height - 1) * 2);

    float yScale = 16.0f / 256.0f;
    float yShift = 4.0f;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const unsigned char y = data.get()[(j + width * i) * nChannels];

            const Eigen::Vector3f position(-height / 2.0f + i,
                                           (int)y * yScale - yShift,
                                           -width / 2.0f + j);

            V.emplace_back(position, Eigen::Vector3f::Zero());
        }
    }

    for (int i = 0; i < height - 1; ++i) {
        for (int j = 0; j < width - 1; ++j) {

            T.emplace_back(i * width + j, i * width + j + 1,
                           i * width + j + width);

            T.emplace_back(i * width + j + width, i * width + j + 1,
                           i * width + j + width + 1);
        }
    }
    recomputeNormals();
}

void Mesh::recomputeNormals() {
    for (unsigned int i = 0; i < V.size(); i++)
        V[i].n = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    for (unsigned int i = 0; i < T.size(); i++) {
        Eigen::Vector3f e01 = V[T[i].v[1]].p - V[T[i].v[0]].p;
        Eigen::Vector3f e02 = V[T[i].v[2]].p - V[T[i].v[0]].p;
        Eigen::Vector3f n = e01.cross(e02).normalized();
        for (unsigned int j = 0; j < 3; j++)
            V[T[i].v[j]].n += n;
    }
    for (unsigned int i = 0; i < V.size(); i++)
        V[i].n.normalize();
}

void Mesh::centerAndScaleToUnit() {
    Eigen::Vector3f c(0.0f, 0.0f, 0.0f);
    for (unsigned int i = 0; i < V.size(); i++)
        c += V[i].p;
    c /= V.size();
    float maxD = (V[0].p - c).norm();
    for (unsigned int i = 0; i < V.size(); i++) {
        float m = (V[i].p - c).norm();
        if (m > maxD)
            maxD = m;
    }
    for (unsigned int i = 0; i < V.size(); i++) {
        V[i].p = (V[i].p - c) / maxD;
        V[i].pInit = (V[i].pInit - c) / maxD;
    }
}