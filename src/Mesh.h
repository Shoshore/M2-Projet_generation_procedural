#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

class Mesh {
  public:
    struct MeshData {
        GLuint vbo;
        GLuint nbo;
        GLuint ubo;
        GLuint ebo;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        std::vector<unsigned int> indices;

        MeshData(const std::vector<glm::vec3> &vertices,
                 const std::vector<unsigned int> &indices,
                 const std::vector<glm::vec3> &normals,
                 const std::vector<glm::vec2> &uvs);
        ~MeshData();

        MeshData(const MeshData &) = delete;
        MeshData &operator=(const MeshData &) = delete;
        MeshData(MeshData &&) = delete;
        MeshData &operator=(MeshData &&) = delete;

        bool hasNormals() const;
        bool hasUVs() const;
        GLsizei indexCount() const;
    };

    Mesh() = default;
    explicit Mesh(std::shared_ptr<MeshData> data);
    Mesh(const std::vector<glm::vec3> &vertices,
         const std::vector<unsigned int> &indices);

    Mesh(const std::vector<glm::vec3> &vertices,
         const std::vector<unsigned int> &indices,
         const std::vector<glm::vec3> &normals);

    Mesh(const std::vector<glm::vec3> &vertices,
         const std::vector<unsigned int> &indices,
         const std::vector<glm::vec2> &uvs);

    Mesh(const std::vector<glm::vec3> &vertices,
         const std::vector<unsigned int> &indices,
         const std::vector<glm::vec3> &normals,
         const std::vector<glm::vec2> &uvs);

    Mesh(const Mesh &) = default;
    Mesh &operator=(const Mesh &) = default;
    Mesh(Mesh &&) noexcept = default;
    Mesh &operator=(Mesh &&) noexcept = default;

    bool hasNormals() const;
    bool hasUVs() const;
    void draw() const;
    void cleanUp();

    std::vector<glm::vec3> getVertices() const;
    std::vector<unsigned int> getIndices() const;
    std::vector<glm::vec3> getNormals() const;
    std::vector<glm::vec2> getUvs() const;
    void updateMeshData(const std::vector<glm::vec3> &vertices,
                        const std::vector<unsigned int> &indices,
                        const std::vector<glm::vec3> &normals,
                        const std::vector<glm::vec2> &uvs);

    void loadOFF(const std::string &filename);
    void loadHeightMap(const std::string &filename);
    void centerAndScaleToUnit();
    void recomputeNormals();

  private:
    std::shared_ptr<MeshData> data;
};

#endif
