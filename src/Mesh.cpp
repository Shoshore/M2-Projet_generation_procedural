#include "Mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include <lib/stb_image.h>

#include <algorithm>
#include <fstream>
#include <memory>

Mesh::MeshData::MeshData(const std::vector<Eigen::Vector3f> &vertices,
                         const std::vector<unsigned int> &indices,
                         const std::vector<Eigen::Vector3f> &normals,
                         const std::vector<Eigen::Vector2f> &uvs)
    : vbo(0), nbo(0), ubo(0), ebo(0), vertices(vertices), normals(normals),
      uvs(uvs), indices(indices) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Eigen::Vector3f),
                 vertices.data(), GL_STATIC_DRAW);

    if (hasNormals()) {
        glGenBuffers(1, &nbo);
        glBindBuffer(GL_ARRAY_BUFFER, nbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Eigen::Vector3f),
                     normals.data(), GL_STATIC_DRAW);
    }

    if (hasUVs()) {
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_ARRAY_BUFFER, ubo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Eigen::Vector2f),
                     uvs.data(), GL_STATIC_DRAW);
    }

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::MeshData::~MeshData() {
    glDeleteBuffers(1, &vbo);
    if (hasNormals()) {
        glDeleteBuffers(1, &nbo);
    }
    if (hasUVs()) {
        glDeleteBuffers(1, &ubo);
    }
    glDeleteBuffers(1, &ebo);
}

bool Mesh::MeshData::hasNormals() const { return !normals.empty(); }
bool Mesh::MeshData::hasUVs() const { return !uvs.empty(); }

GLsizei Mesh::MeshData::indexCount() const {
    return static_cast<GLsizei>(indices.size());
}

Mesh::Mesh(std::shared_ptr<MeshData> data) : data(std::move(data)) {}

Mesh::Mesh(const std::vector<Eigen::Vector3f> &vertices,
           const std::vector<unsigned int> &indices)
    : Mesh(std::make_shared<MeshData>(vertices, indices,
                                      std::vector<Eigen::Vector3f>{},
                                      std::vector<Eigen::Vector2f>{})) {}

Mesh::Mesh(const std::vector<Eigen::Vector3f> &vertices,
           const std::vector<unsigned int> &indices,
           const std::vector<Eigen::Vector3f> &normals)
    : Mesh(std::make_shared<MeshData>(vertices, indices, normals,
                                      std::vector<Eigen::Vector2f>{})) {}

Mesh::Mesh(const std::vector<Eigen::Vector3f> &vertices,
           const std::vector<unsigned int> &indices,
           const std::vector<Eigen::Vector2f> &uvs)
    : Mesh(std::make_shared<MeshData>(vertices, indices,
                                      std::vector<Eigen::Vector3f>{}, uvs)) {}

Mesh::Mesh(const std::vector<Eigen::Vector3f> &vertices,
           const std::vector<unsigned int> &indices,
           const std::vector<Eigen::Vector3f> &normals,
           const std::vector<Eigen::Vector2f> &uvs)
    : Mesh(std::make_shared<MeshData>(vertices, indices, normals, uvs)) {}

bool Mesh::hasNormals() const { return data && data->hasNormals(); }
bool Mesh::hasUVs() const { return data && data->hasUVs(); }

void Mesh::draw() const {
    if (!data) {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, data->vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, nullptr);

    if (hasNormals()) {
        glBindBuffer(GL_ARRAY_BUFFER, data->nbo);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, nullptr);
    }

    if (hasUVs()) {
        glBindBuffer(GL_ARRAY_BUFFER, data->ubo);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->ebo);
    glDrawElements(GL_TRIANGLES, data->indexCount(), GL_UNSIGNED_INT, nullptr);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (hasNormals()) {
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    if (hasUVs()) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::cleanUp() { data.reset(); }

std::vector<Eigen::Vector3f> Mesh::getVertices() const {
    return data ? data->vertices : std::vector<Eigen::Vector3f>{};
}

std::vector<unsigned int> Mesh::getIndices() const {
    return data ? data->indices : std::vector<unsigned int>{};
}

std::vector<Eigen::Vector3f> Mesh::getNormals() const {
    return data ? data->normals : std::vector<Eigen::Vector3f>{};
}

std::vector<Eigen::Vector2f> Mesh::getUvs() const {
    return data ? data->uvs : std::vector<Eigen::Vector2f>{};
}

void Mesh::updateMeshData(const std::vector<Eigen::Vector3f> &vertices,
                          const std::vector<unsigned int> &indices,
                          const std::vector<Eigen::Vector3f> &normals,
                          const std::vector<Eigen::Vector2f> &uvs) {
    data = std::make_shared<MeshData>(vertices, indices, normals, uvs);
}

void Mesh::loadOFF(const std::string &filename) {
    std::ifstream in(filename.c_str());
    std::string offString;
    unsigned int vertexCount, triangleCount, tmp;
    in >> offString >> vertexCount >> triangleCount >> tmp;

    std::vector<Eigen::Vector3f> vertices(vertexCount);
    std::vector<unsigned int> indices(triangleCount * 3);
    for (Eigen::Vector3f &vertex : vertices) {
        in >> vertex[0] >> vertex[1] >> vertex[2];
    }
    for (unsigned int i = 0; i < triangleCount; ++i) {
        int faceSize;
        in >> faceSize;
        in >> indices[i * 3] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
    }

    updateMeshData(vertices, indices, {}, {});
    centerAndScaleToUnit();
    recomputeNormals();
}

void Mesh::loadHeightMap(const std::string &filename) {
    int width = 0;
    int height = 0;
    int nChannels = 0;
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> image(
        stbi_load(filename.c_str(), &width, &height, &nChannels, 0),
        stbi_image_free);

    std::vector<Eigen::Vector3f> vertices;
    std::vector<unsigned int> indices;
    vertices.reserve(static_cast<std::size_t>(width) * height);
    indices.reserve(static_cast<std::size_t>(width - 1) * (height - 1) * 6);

    const float yScale = 64.0f / 256.0f;
    const float yShift = 16.0f;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const unsigned char y = image.get()[(j + width * i) * nChannels];
            vertices.emplace_back(-height / 2.0f + i, y * yScale - yShift,
                                  -width / 2.0f + j);
        }
    }

    for (int i = 0; i < height - 1; ++i) {
        for (int j = 0; j < width - 1; ++j) {
            indices.insert(
                indices.end(),
                {static_cast<unsigned int>(i * width + j),
                 static_cast<unsigned int>(i * width + j + 1),
                 static_cast<unsigned int>(i * width + j + width),
                 static_cast<unsigned int>(i * width + j + width),
                 static_cast<unsigned int>(i * width + j + 1),
                 static_cast<unsigned int>(i * width + j + width + 1)});
        }
    }

    updateMeshData(vertices, indices, {}, {});
    recomputeNormals();
}

void Mesh::centerAndScaleToUnit() {
    if (!data || data->vertices.empty()) {
        return;
    }

    std::vector<Eigen::Vector3f> vertices = data->vertices;

    Eigen::Vector3f center = Eigen::Vector3f::Zero();

    for (const Eigen::Vector3f &vertex : vertices) {
        center += vertex;
    }

    center /= static_cast<float>(vertices.size());

    float maxDistance = (vertices[0] - center).norm();

    for (const Eigen::Vector3f &vertex : vertices) {
        maxDistance = std::max(maxDistance, (vertex - center).norm());
    }

    if (maxDistance > 0.0f) {
        for (Eigen::Vector3f &vertex : vertices) {
            vertex = (vertex - center) / maxDistance;
        }
    }

    updateMeshData(vertices, data->indices, data->normals, data->uvs);
}

void Mesh::recomputeNormals() {
    if (!data) {
        return;
    }

    std::vector<Eigen::Vector3f> normals(
        data->vertices.size(), Eigen::Vector3f::Zero());

    for (std::size_t i = 0; i < data->indices.size(); i += 3) {
        const unsigned int i0 = data->indices[i];
        const unsigned int i1 = data->indices[i + 1];
        const unsigned int i2 = data->indices[i + 2];

        Eigen::Vector3f normal =
            (data->vertices[i1] - data->vertices[i0])
                .cross(data->vertices[i2] - data->vertices[i0]);

        if (normal.squaredNorm() > 1e-12f) {
            normal.normalize();
        }

        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }

    for (Eigen::Vector3f &normal : normals) {
        if (normal.squaredNorm() > 1e-12f) {
            normal.normalize();
        }
    }

    updateMeshData(data->vertices, data->indices, normals, data->uvs);
}
