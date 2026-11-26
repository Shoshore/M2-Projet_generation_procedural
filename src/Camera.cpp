#include "Camera.h"


Camera::Camera(const Eigen::Vector3f& position, int width, int height)
    : m_position(position), m_orientation(Eigen::Quaternionf::Identity()) {
    updateOrientation();
    resize(width, height);
}


void Camera::updateOrientation() {
    Eigen::Quaternionf qYaw(Eigen::AngleAxisf(m_yaw, VEC_UP));
    Eigen::Quaternionf qPitch(Eigen::AngleAxisf(m_pitch, VEC_RIGHT));
    m_orientation = (qYaw * qPitch).normalized();
}


void Camera::rotate(float dxMouse, float dyMouse) {
    m_yaw -= dxMouse * MOUSE_SPEED;
    m_pitch -= dyMouse * MOUSE_SPEED;
    m_pitch = std::clamp(m_pitch, -MAX_PITCH, MAX_PITCH);
    updateOrientation();
}


Eigen::Vector3f Camera::front() const { return m_orientation * VEC_FRONT; }
Eigen::Vector3f Camera::right() const { return m_orientation * VEC_RIGHT; }
Eigen::Vector3f Camera::up() const { return m_orientation * VEC_UP; }


void Camera::moveForward() { m_position += front() * MOVEMENT_SPEED; }
void Camera::moveBackward() { m_position -= front() * MOVEMENT_SPEED; }
void Camera::moveRight() { m_position += right() * MOVEMENT_SPEED; }
void Camera::moveLeft() { m_position -= right() * MOVEMENT_SPEED; }
void Camera::moveUp() { m_position += VEC_UP * MOVEMENT_SPEED; }
void Camera::moveDown() { m_position -= VEC_UP * MOVEMENT_SPEED; }


Eigen::Matrix4f Camera::viewMatrix() const {
    Eigen::Vector3f r = right();
    Eigen::Vector3f u = up();
    Eigen::Vector3f f = front();

    Eigen::Matrix3f R; 
    R.row(0) = r.transpose();
    R.row(1) = u.transpose();
    R.row(2) = -f.transpose();

    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
    view.block<3,3>(0,0) = R;
    view.block<3,1>(0,3) = -R * m_position;
    return view;
}


void Camera::resize(int width, int height) {
    m_width  = width;
    m_height = height;
    m_aspect = static_cast<float>(width) / static_cast<float>(height);
    updateProjection();
}

void Camera::setFov(float fovDegrees) {
    m_fov = fovDegrees * 3.14159265f / 180.0f;
    updateProjection();
}

void Camera::updateProjection() {
    const float f = 1.0f / std::tan(m_fov * 0.5f);

    m_projection.setZero();
    m_projection(0, 0) = f / m_aspect;
    m_projection(1, 1) = f;
    m_projection(2, 2) = (m_far + m_near) / (m_near - m_far);
    m_projection(2, 3) = (2.0f * m_far * m_near) / (m_near - m_far);
    m_projection(3, 2) = -1.0f;
}