#include "camera.hpp"

using namespace LuGL;

Camera::Camera()
{
    m_position  = vec3(0.0f, 0.0f, 1.0f);
    m_target    = vec3(0.0f, 0.0f, 0.0f);
    //
    // note : this step may be called before the static variables initialized in maths.cpp
    // so do not use static variables like vec3::UNIT_Y
    // which will cause a BUG in certain cases
    //
    m_up        = vec3(0.0f, 1.0f, 0.0f);
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

void Camera::setAspect(float aspect)
{
    m_aspect = aspect;
}

void Camera::setFOV(float fov)
{
    m_FOV = fov;
}
void Camera::setNear(float near)
{
    m_near = near;
}
void Camera::setFar(float far)
{
    m_far = far;
}
void Camera::setUp(const Vector3 & up)
{
    m_up = up;
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
float Camera::getFOV() const
{
    return m_FOV;
}

Matrix4 Camera::getViewMatrix() const
{
    return Matrix4::fromLookAt(m_position, m_target, m_up);
}
Matrix4 Camera::getProjectMatrix() const
{
    return Matrix4::getProjection(m_FOV, m_aspect, m_near, m_far);
}

void Camera::rotateByDrag(float delta_x, float delta_y, float drag_speed)
{
    vec3 dir = m_position - m_target;
    vec3 y_axis = -dir.cross(m_up).normalized();
    vec3 x_axis = y_axis.cross(dir).normalized();
    m_up = -x_axis;

    if (fabs(delta_x) > EPSILON)
    {
        Quaternion rotate_x = Quaternion::fromAxisAngle(x_axis, delta_x * drag_speed);
        dir.rotate(rotate_x);
    }

    if (fabs(delta_y) > EPSILON)
    {
        Quaternion rotate_y = Quaternion::fromAxisAngle(y_axis, delta_y * drag_speed);
        dir.rotate(rotate_y);
    }

    m_position = m_target + dir;
}
