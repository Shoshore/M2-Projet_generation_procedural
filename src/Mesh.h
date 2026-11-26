#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <Eigen>
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

        std::vector<Eigen::Vector3f> vertices;
        std::vector<Eigen::Vector3f> normals;
        std::vector<Eigen::Vector2f> uvs;
        std::vector<unsigned int> indices;

        MeshData(const std::vector<Eigen::Vector3f> &vertices,
                 const std::vector<unsigned int> &indices,
                 const std::vector<Eigen::Vector3f> &normals,
                 const std::vector<Eigen::Vector2f> &uvs);
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
    Mesh(const std::vector<Eigen::Vector3f> &vertices,
         const std::vector<unsigned int> &indices);

    Mesh(const std::vector<Eigen::Vector3f> &vertices,
         const std::vector<unsigned int> &indices,
         const std::vector<Eigen::Vector3f> &normals);

    Mesh(const std::vector<Eigen::Vector3f> &vertices,
         const std::vector<unsigned int> &indices,
         const std::vector<Eigen::Vector2f> &uvs);

    Mesh(const std::vector<Eigen::Vector3f> &vertices,
         const std::vector<unsigned int> &indices,
         const std::vector<Eigen::Vector3f> &normals,
         const std::vector<Eigen::Vector2f> &uvs);

    Mesh(const Mesh &) = default;
    Mesh &operator=(const Mesh &) = default;
    Mesh(Mesh &&) noexcept = default;
    Mesh &operator=(Mesh &&) noexcept = default;

    bool hasNormals() const;
    bool hasUVs() const;
    void draw() const;
    void cleanUp();

    std::vector<Eigen::Vector3f> getVertices() const;
    std::vector<unsigned int> getIndices() const;
    std::vector<Eigen::Vector3f> getNormals() const;
    std::vector<Eigen::Vector2f> getUvs() const;
    void updateMeshData(const std::vector<Eigen::Vector3f> &vertices,
                        const std::vector<unsigned int> &indices,
                        const std::vector<Eigen::Vector3f> &normals,
                        const std::vector<Eigen::Vector2f> &uvs);

    void loadOFF(const std::string &filename);
    void loadHeightMap(const std::string &filename);
    void centerAndScaleToUnit();
    void recomputeNormals();

  private:
    std::shared_ptr<MeshData> data;
};

#endif
