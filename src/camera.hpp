#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "maths.hpp"

namespace LuGL
{

#define DRAG_SPEED 0.02f

class Camera
{
private:
    Vector3 m_position;
    Vector3 m_target;
    Vector3 m_up;
    float   m_FOV;
    float   m_aspect;
    float   m_near;
    float   m_far;
public:
    Camera();
    void setTransform(const Vector3 & position, const Vector3 & target);
    void setAspect(float aspect);
    void setFOV(float fov);
    void setNear(float near);
    void setFar(float far);
    void setUp(const Vector3 & up);
    Vector3 getPosition() const;
    Vector3 getForward() const;
    Vector3 getUp() const;
    float getFOV() const;
    Matrix4 getViewMatrix() const;
    Matrix4 getProjectMatrix() const;

    void rotateByDrag(float delta_x, float delta_y, float drag_speed = DRAG_SPEED);
};

}

#endif