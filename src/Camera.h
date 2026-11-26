#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cmath>
class Camera {
  public:
    Camera(const Eigen::Vector3f& position = Eigen::Vector3f::Zero(),
           int width = 800, int height = 600);

    void resize(int width, int height);
    void rotate(float dxMouse, float dyMouse);

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    Eigen::Vector3f front() const;
    Eigen::Vector3f right() const;
    Eigen::Vector3f up() const;

    const Eigen::Vector3f& position() const { return m_position; }
    const Eigen::Quaternionf& orientation() const { return m_orientation; }

    Eigen::Matrix4f viewMatrix() const;
    const Eigen::Matrix4f& projectionMatrix() const { return m_projection; }

    void setFov(float fovDegrees);
    float aspect() const { return m_aspect; }

  private:
    void updateOrientation();
    void updateProjection();

    Eigen::Vector3f m_position;
    Eigen::Quaternionf m_orientation;
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;

    Eigen::Matrix4f m_projection = Eigen::Matrix4f::Identity();
    int m_width = 800;
    int m_height = 600;
    float m_aspect = 800.0f / 600.0f;
    float m_fov = 70.0f * M_PI / 180.0f;
    float m_near = 0.1f;
    float m_far = 1000.0f;

    const Eigen::Vector3f VEC_FRONT = {1.0f, 0.0f, 0.0f};
    const Eigen::Vector3f VEC_UP = {0.0f, 1.0f, 0.0f};
    const Eigen::Vector3f VEC_RIGHT = {0.0f, 0.0f, 1.0f};

    const float MOUSE_SPEED = 0.0005f;
    const float MOVEMENT_SPEED = 2.0f;
    const float MAX_PITCH = 89.0f * M_PI / 180.0f;
};

#endif