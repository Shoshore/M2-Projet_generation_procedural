#include "Mesh.h"
#include <fstream>

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