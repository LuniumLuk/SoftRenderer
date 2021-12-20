#include "camera.hpp"

using namespace Lurdr;

Camera::Camera()
{
    m_position  = Vector3::UNIT_Z;
    m_target    = Vector3::ZERO;
    m_up        = Vector3::UNIT_Y;
    m_FOV       = PI / 3; // 60 degree
    m_aspect    = 1.0f;
    m_near      = 0.1f;
    m_far       = 1000.0f;
}

void Camera::setTransform(const Vector3 & position, const Vector3 & target)
{
    assert((target - position).length() > EPSILON);
    m_position = position;
    m_target = target;
}

Vector3 Camera::getPosition() const
{
    return m_position;
}
Vector3 Camera::getForward() const
{
    return (m_target - m_position).normalized();
}
Vector3 Camera::getUp() const
{
    return m_up;
}

Matrix4 Camera::getViewMatrix() const
{
    return Matrix4::fromLookAt(m_position, m_target, m_up);
}
Matrix4 Camera::getProjectMatrix() const
{
    return Matrix4::getProjection(m_FOV, m_aspect, m_near, m_far);
}