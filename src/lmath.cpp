#include "../include/lmath.h"

using namespace Lurdr;

/**
 * TODOs :
 *  - check normalization
 *  - slerp() implementation
 *  - check correctness
 *  - check EPSILON
 */

/**
 * Vector2 : Functionalities and Utilities
 */

Vector2 Vector2::UNIT_X = Vector2(1.0, 0.0);
Vector2 Vector2::UNIT_Y = Vector2(0.0, 1.0);
Vector2 Vector2::ZERO   = Vector2(0.0, 0.0);

Vector2 Vector2::operator+ (const Vector2 & other) const {
    Vector2 vec(x + other.x, y + other.y);
    return vec;
}
Vector2 Vector2::operator- () const {
    Vector2 vec(-x, -y);
    return vec;
}
Vector2 Vector2::operator- (const Vector2 & other) const {
    Vector2 vec(x - other.x, y - other.y);
    return vec;
}
Vector2 operator* (float multiplier, const Vector2 & multiplicand) {
    Vector2 vec(
        multiplier * multiplicand.x,
        multiplier * multiplicand.y
    );
    return vec;
}
Vector2 Vector2::operator* (const float & multiplicand) const {
    Vector2 vec(x * multiplicand, y * multiplicand);
    return vec;
}
Vector2 Vector2::operator/ (const float divisor) const {
    assert(divisor != 0);
    float factor = 1.0 / divisor;
    Vector2 vec(x * factor, y * factor);
    return vec;
}
bool Vector2::operator== (const Vector2 & other) const {
    if (x == other.x && y == other.y) {
        return true;
    } else {
        return false;
    }
}
bool Vector2::operator!= (const Vector2 & other) const {
    if (*this == other) {
        return false;
    } else {
        return true;
    }
}
float Vector2::dot(const Vector2 & other) const {
    return x * other.x + y * other.y;
}
Vector2 Vector2::cross(const Vector2 & other) const {
    Vector2 vec(
        y - other.x,
        x - other.y );
    return vec;
}
Vector2 Vector2::lerp(const Vector2 & from, const Vector2 & to, float alpha) {
    alpha = clamp(alpha, 0.0, 1.0);
    Vector2 vec(
        from.x * (1 - alpha) + to.x * alpha,
        from.y * (1 - alpha) + to.y * alpha
    );
    return vec;
}
float Vector2::length() const {
    return sqrtf(x * x + y * y);
}
void Vector2::print() {
    printf("Vector3 : ( %6f, %6f )\n", x, y);
}

Vector2 Vector2::normalized() const {
    float len = length();
    assert(len != 0);
    float factor = 1.0 / len;
    Vector2 vec(
        x * factor,
        y * factor
    );
    return vec;
}
void Vector2::normalize() {
    float len = length();
    assert(len != 0);
    float factor = 1.0 / len;
    x *= factor;
    y *= factor;
}

/**
 * Vector3 : Functionalities and Utilities
 */

Vector3 Vector3::UNIT_X = Vector3(1.0, 0.0, 0.0);
Vector3 Vector3::UNIT_Y = Vector3(0.0, 1.0, 0.0);
Vector3 Vector3::UNIT_Z = Vector3(0.0, 0.0, 1.0);
Vector3 Vector3::ZERO   = Vector3(0.0, 0.0, 0.0);

Vector3 Vector3::operator+ (const Vector3 & other) const {
    Vector3 vec(x + other.x, y + other.y, z + other.z);
    return vec;
}
Vector3 Vector3::operator- () const {
    Vector3 vec(-x, -y, -z);
    return vec;
}
Vector3 Vector3::operator- (const Vector3 & other) const {
    Vector3 vec(x - other.x, y - other.y, z - other.z);
    return vec;
}
Vector3 operator* (float multiplier, const Vector3 & multiplicand) {
    Vector3 vec(
        multiplier * multiplicand.x,
        multiplier * multiplicand.y,
        multiplier * multiplicand.z
    );
    return vec;
}
Vector3 Vector3::operator* (const float & multiplicand) const {
    Vector3 vec(x * multiplicand, y * multiplicand, z * multiplicand);
    return vec;
}
Vector3 Vector3::operator/ (const float divisor) const {
    assert(divisor != 0);
    float factor = 1.0 / divisor;
    Vector3 vec(x * factor, y * factor, z * factor);
    return vec;
}
bool Vector3::operator== (const Vector3 & other) const {
    if (x == other.x && y == other.y && z == other.z) {
        return true;
    } else {
        return false;
    }
}
bool Vector3::operator!= (const Vector3 & other) const {
    if (*this == other) {
        return false;
    } else {
        return true;
    }
}
float Vector3::dot(const Vector3 & other) const {
    return x * other.x + y * other.y + z * other.z;
}
// reference : https://en.wikipedia.org/wiki/Cross_product
Vector3 Vector3::cross(const Vector3 & other) const {
    Vector3 vec(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x );
    return vec;
}
float Vector3::length() const {
    return sqrtf(x * x + y * y + z * z);
}
Vector3 Vector3::normalized() const {
    float len = length();
    assert(len != 0);
    float factor = 1.0 / len;
    Vector3 vec(
        x * factor,
        y * factor,
        z * factor
    );
    return vec;
}
void Vector3::normalize() {
    float len = length();
    assert(len != 0);
    float factor = 1.0 / len;
    x *= factor;
    y *= factor;
    z *= factor;
}
void Vector3::print() {
    printf("Vector3 : ( %6f, %6f, %6f )\n", x, y, z);
}
// reference : https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
Vector3 Vector3::rotated(const Quaternion & quat) const {
    Vector3 u(quat.x, quat.y, quat.z);
    float s = quat.w;

    Vector3 vec = 2.0f * u.dot(*this) * u
                + (s * s - u.dot(u)) * (*this)
                + 2.0f * s * u.cross(*this);
    return vec;
}
void Vector3::rotate(const Quaternion & quat) {
    Vector3 u(quat.x, quat.y, quat.z);
    float s = quat.w;
    
    *this = 2.0f * u.dot(*this) * u
          + (s * s - u.dot(u)) * (*this)
          + 2.0f * s * u.cross(*this);
}

Vector3 Vector3::Vector3::lerp(const Vector3 & from, const Vector3 & to, float alpha) {
    alpha = clamp(alpha, 0.0, 1.0);
    Vector3 vec(
        from.x * (1 - alpha) + to.x * alpha,
        from.y * (1 - alpha) + to.y * alpha,
        from.z * (1 - alpha) + to.z * alpha
    );
    return vec;
}

/**
 * Quaternion : Functionalities and Utilities
 * API reference : https://docs.unity.cn/2019.4/Documentation/ScriptReference/Quaternion.html
 */

Quaternion Quaternion::IDENTITY = Quaternion(1.0, 0.0, 0.0, 0.0);

// reference : https://en.wikipedia.org/wiki/Quaternion
// & https://ww2.mathworks.cn/help/aeroblks/quaternionmultiplication.html
// & http://kieranwynn.github.io/pyquaternion/ for cross reference
// note : 'a + bi + cj + dk' <=> 'w + xi + yj + zk'
Quaternion Quaternion::operator* (const Quaternion & other) const {
    Quaternion quat(
        other.w * w - other.x * x - other.y * y - other.z * z,
        other.w * x + other.x * w - other.y * z + other.z * y,
        other.w * y + other.x * z + other.y * w - other.z * x,
        other.w * z - other.x * y + other.y * x + other.z * w
    );
    return quat;
}
bool Quaternion::operator== (const Quaternion & other) const {
    if (w == other.w && x == other.x && y == other.y && z == other.z) {
        return true;
    } else {
        return false;
    }
}
bool Quaternion::operator!= (const Quaternion & other) const {
    if (*this == other) {
        return false;
    } else {
        return true;
    }
}
float Quaternion::dot(const Quaternion & other) const {
    return w * other.w + x * other.x + y * other.y + z * other.z;
}
Quaternion Quaternion::normalized() const {
    float len = sqrtf(dot(*this));
    assert(len != 0);
    float factor = 1.0 / len;
    Quaternion quat(
        w * factor,
        x * factor,
        y * factor,
        z * factor
    );
    return quat;
}
void Quaternion::normalize() {
    float len = sqrtf(dot(*this));
    assert(len != 0);
    float factor = 1.0 / len;
    w *= factor;
    x *= factor;
    y *= factor;
    z *= factor;
}
// reference : https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// & https://personal.utdallas.edu/~sxb027100/dock/quaternion.html
Vector3 Quaternion::toEulerAngles() const {
    float roll = atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y));
    float p = 2 * (w * y - x * z);
    float pitch;
    if (abs(p) >= 1) {
        // handle 90 degree rotation
        pitch = copysign(PI / 2, p);
    } else {
        pitch = asin(p);
    }
    float yaw = atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z));

    Vector3 vec(roll, pitch, yaw);
    return vec;
}
// nonzero quaternion has an inverse with respect to its Hamilton product
Quaternion Quaternion::inverse() const {
    float q = dot(*this);
    assert(q != 0);
    float factor = 1.0 / q;
    Quaternion quat(
         w * factor,
        -x * factor,
        -y * factor,
        -z * factor
    );
    return quat;
}
void Quaternion::print() {
    printf("Quaternion : ( %6f, %6f, %6f, %6f )\n", w, x, y, z);
}
// linear lerp for quaternion
Quaternion Quaternion::lerp(const Quaternion & from, const Quaternion & to, float alpha) {
    alpha = clamp(alpha, 0.0, 1.0);
    Quaternion quat(
        from.w * (1 - alpha) + to.w * alpha,
        from.x * (1 - alpha) + to.x * alpha,
        from.y * (1 - alpha) + to.y * alpha,
        from.z * (1 - alpha) + to.z * alpha
    );
    return quat;
}
// spherical lerp for quaternion
Quaternion Quaternion::slerp(const Quaternion & from, const Quaternion & to, float alpha) {
    Quaternion quat;
    bool is_implemented = false;
    assert(is_implemented);
    // ....
    return quat;
}
// reference : https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Quaternion Quaternion::fromEulerAngles(const Vector3 & angles) {
    float roll  = angles.x;
    float pitch = angles.y;
    float yaw   = angles.z;

    float cy = cos(yaw   * 0.5f);
    float sy = sin(yaw   * 0.5f);
    float cp = cos(pitch * 0.5f);
    float sp = sin(pitch * 0.5f);
    float cr = cos(roll  * 0.5f);
    float sr = sin(roll  * 0.5f);

    Quaternion quat(
        cr * cp * cy + sr * sp * sy,
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy
    );
    return quat;
}
// reference : https://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another
Quaternion Quaternion::fromToRotation(const Vector3 & from, const Vector3 to) {
    Vector3 from_n = from.normalized();
    Vector3 to_n = to.normalized();
    float dot = from_n.dot(to_n);
    if (dot > 1.0f - EPSILON) {
        // parallel vectors pointing the same direction
        return IDENTITY;
    } else if (dot < -1.0f + EPSILON) {
        // parallel vectors pointing opposite direction
        return fromAxisAngle(Vector3::UNIT_X, PI);
    } else {
        Vector3 cross = from_n.cross(to_n);
        Quaternion quat(
            1.0f + dot,
            cross.x,
            cross.y,
            cross.z
        );
        return quat.normalized();
    }
}
// note : angle is mesured in radiance
Quaternion Quaternion::fromAxisAngle(const Vector3 & axis, float angle) {
    float cosa = cosf(angle * 0.5f);
    float sina = sinf(angle * 0.5f);
    Quaternion quat(
        cosa,
        sina * axis.x,
        sina * axis.y,
        sina * axis.z
    );
    return quat;
}

