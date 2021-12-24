#include "maths.hpp"

using namespace Lurdr;

/**
 * TODOs :
 *  - check normalization
 *  - check correctness
 *  - check EPSILON
 */

/**
 * Vector2 : functionalities and utilities
 */

Vector2 Vector2::UNIT_X = Vector2( 1.0f, 0.0f );
Vector2 Vector2::UNIT_Y = Vector2( 0.0f, 1.0f );
Vector2 Vector2::ZERO   = Vector2( 0.0f, 0.0f );

void Vector2::operator= (const Vector2 & other)
{
    x = other.x;
    y = other.y;
}
Vector2 Vector2::operator+ (const Vector2 & other) const
{
    Vector2 vec(x + other.x, y + other.y);
    return vec;
}
Vector2 Vector2::operator- () const
{
    Vector2 vec(-x, -y);
    return vec;
}
Vector2 Vector2::operator- (const Vector2 & other) const
{
    Vector2 vec(x - other.x, y - other.y);
    return vec;
}
Vector2 Lurdr::operator* (float multiplier, const Vector2 & multiplicand)
{
    Vector2 vec(
        multiplier * multiplicand.x,
        multiplier * multiplicand.y
    );
    return vec;
}
Vector2 Vector2::operator* (const float & multiplicand) const
{
    Vector2 vec(x * multiplicand, y * multiplicand);
    return vec;
}
Vector2 Vector2::operator/ (const float divisor) const
{
    assert(fabs(divisor) > EPSILON);
    float factor = 1.0f / divisor;
    Vector2 vec(x * factor, y * factor);
    return vec;
}
bool Vector2::operator== (const Vector2 & other) const
{
    if ( fabs(x - other.x) <= EPSILON * max(max(fabs(x), fabs(other.x)), 1.0f)
      && fabs(y - other.y) <= EPSILON * max(max(fabs(y), fabs(other.y)), 1.0f) )
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Vector2::operator!= (const Vector2 & other) const
{
    if (*this == other)
    {
        return false;
    }
    else
    {
        return true;
    }
}
void Vector2::operator+= (const Vector2 & other)
{
    x += other.x;
    y += other.y;
}
float Vector2::dot(const Vector2 & other) const
{
    return x * other.x + y * other.y;
}
float Vector2::cross(const Vector2 & other) const
{
    return x * other.y - y * other.x;
}
Vector2 Vector2::lerp(const Vector2 & from, const Vector2 & to, float alpha)
{
    alpha = clamp(alpha, 0.0, 1.0);
    Vector2 vec(
        from.x * (1.0f - alpha) + to.x * alpha,
        from.y * (1.0f - alpha) + to.y * alpha
    );
    return vec;
}
float Vector2::length() const
{
    return sqrtf(x * x + y * y);
}
void Vector2::print() const
{
    printf("Vector3 : ( %9.3f, %9.3f )\n", x, y);
}

Vector2 Vector2::normalized() const
{
    float len = length();
    assert(fabs(len) > EPSILON);
    float factor = 1.0f / len;
    Vector2 vec(
        x * factor,
        y * factor
    );
    return vec;
}
void Vector2::normalize()
{
    float len = length();
    assert(fabs(len) > EPSILON);
    float factor = 1.0f / len;
    x *= factor;
    y *= factor;
}

/**
 * Vector3 : Functionalities and Utilities
 */

Vector3 Vector3::UNIT_X = Vector3( 1.0f, 0.0f, 0.0f );
Vector3 Vector3::UNIT_Y = Vector3( 0.0f, 1.0f, 0.0f );
Vector3 Vector3::UNIT_Z = Vector3( 0.0f, 0.0f, 1.0f );
Vector3 Vector3::ZERO   = Vector3( 0.0f, 0.0f, 0.0f );

void Vector3::operator= (const Vector3 & other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}
Vector3 Vector3::operator+ (const Vector3 & other) const
{
    Vector3 vec(x + other.x, y + other.y, z + other.z);
    return vec;
}
Vector3 Vector3::operator- () const
{
    Vector3 vec(-x, -y, -z);
    return vec;
}
Vector3 Vector3::operator- (const Vector3 & other) const
{
    Vector3 vec(x - other.x, y - other.y, z - other.z);
    return vec;
}
Vector3 Lurdr::operator* (float multiplier, const Vector3 & multiplicand)
{
    Vector3 vec(
        multiplier * multiplicand.x,
        multiplier * multiplicand.y,
        multiplier * multiplicand.z
    );
    return vec;
}
void Lurdr::swap(Vector3 & a, Vector3 & b)
{
    std::swap(a.x, b.x);
    std::swap(a.y, b.y);
    std::swap(a.z, b.z);
}
Vector3 Vector3::operator* (const float & multiplicand) const
{
    Vector3 vec(x * multiplicand, y * multiplicand, z * multiplicand);
    return vec;
}
Vector3 Vector3::operator/ (const float divisor) const
{
    assert(fabs(divisor) > EPSILON);
    float factor = 1.0f / divisor;
    Vector3 vec(x * factor, y * factor, z * factor);
    return vec;
}
bool Vector3::operator== (const Vector3 & other) const
{
    if ( fabs(x - other.x) <= EPSILON * max(max(fabs(x), fabs(other.x)), 1.0f)
      && fabs(y - other.y) <= EPSILON * max(max(fabs(y), fabs(other.y)), 1.0f)
      && fabs(z - other.z) <= EPSILON * max(max(fabs(z), fabs(other.z)), 1.0f) )
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Vector3::operator!= (const Vector3 & other) const
{
    if (*this == other)
    {
        return false;
    }
    else
    {
        return true;
    }
}
void Vector3::operator+= (const Vector3 & other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}
float Vector3::dot(const Vector3 & other) const
{
    return x * other.x + y * other.y + z * other.z;
}
// reference : https://en.wikipedia.org/wiki/Cross_product
Vector3 Vector3::cross(const Vector3 & other) const
{
    Vector3 vec(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x );
    return vec;
}
float Vector3::length() const
{
    return sqrtf(x * x + y * y + z * z);
}
Vector3 Vector3::normalized() const
{
    float len = length();
    assert(fabs(len) > EPSILON);
    float factor = 1.0f / len;
    Vector3 vec(
        x * factor,
        y * factor,
        z * factor
    );
    return vec;
}
void Vector3::normalize()
{
    float len = length();
    assert(fabs(len) > EPSILON);
    float factor = 1.0f / len;
    x *= factor;
    y *= factor;
    z *= factor;
}
void Vector3::print() const
{
    printf("Vector3 : ( %9.3f, %9.3f, %9.3f )\n", x, y, z);
}
// reference : https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
Vector3 Vector3::rotated(const Quaternion & quat) const
{
    Vector3 u(quat.x, quat.y, quat.z);
    float s = quat.w;

    Vector3 vec = 2.0f * u.dot(*this) * u
                + (s * s - u.dot(u)) * (*this)
                + 2.0f * s * u.cross(*this);
    return vec;
}
void Vector3::rotate(const Quaternion & quat)
{
    Vector3 u(quat.x, quat.y, quat.z);
    float s = quat.w;
    
    *this = 2.0f * u.dot(*this) * u
          + (s * s - u.dot(u)) * (*this)
          + 2.0f * s * u.cross(*this);
}

Vector3 Vector3::lerp(const Vector3 & from, const Vector3 & to, float alpha)
{
    alpha = clamp(alpha, 0.0f, 1.0f);
    Vector3 vec(
        from.x * (1 - alpha) + to.x * alpha,
        from.y * (1 - alpha) + to.y * alpha,
        from.z * (1 - alpha) + to.z * alpha
    );
    return vec;
}
// this method is validated by : https://www.vcalc.com/wiki/vCalc/V3+-+Vector+Rotation
Vector3 Vector3::rotatedFromAxisAngle(const Vector3 & axis, const float angle)
{
    float sina = sinf(angle);
    float cosa = cosf(angle);
    Vector3 normal = axis.normalized();

    // vector operation method 1000000 ~= 88ms
    // return (*this) * cosa - (*this).dot(normal) * normal * (cosa - 1) + normal.cross(*this) * sina;

    // numerical method 1000000 ~= 50ms
    float nx = normal.x;
    float ny = normal.y;
    float nz = normal.z;

    float nx2 = nx * nx;
    float ny2 = ny * ny;
    float nz2 = nz * nz;

    float nxy = nx * ny;
    float nxz = nx * nz;
    float nyz = ny * nz;

    Vector3 vec(
        x * cosa + (x * nx2 + y * nxy + z * nxz) * (1 - cosa) + (ny * z - nz * y) * sina,
        y * cosa + (x * nxy + y * ny2 + z * nyz) * (1 - cosa) + (nz * x - nx * z) * sina,
        z * cosa + (x * nxz + y * nyz + z * nz2) * (1 - cosa) + (nx * y - ny * x) * sina
    );
    return vec;
}

/**
 * Vector4 : functionalities and utilities
 */

Vector4 Vector4::UNIT_X = Vector4( 1.0f, 0.0f, 0.0f, 0.0f );
Vector4 Vector4::UNIT_Y = Vector4( 0.0f, 1.0f, 0.0f, 0.0f );
Vector4 Vector4::UNIT_Z = Vector4( 0.0f, 0.0f, 1.0f, 0.0f );
Vector4 Vector4::UNIT_W = Vector4( 0.0f, 0.0f, 0.0f, 1.0f );
Vector4 Vector4::ZERO   = Vector4( 0.0f, 0.0f, 0.0f, 0.0f );

void Vector4::operator= (const Vector4 & other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
}
Vector4 Vector4::operator+ (const Vector4 & other) const
{
    Vector4 vec(x + other.x, y + other.y, z + other.z, w + other.w);
    return vec;
}
Vector4 Vector4::operator- () const
{
    Vector4 vec(-x, -y, -z, -w);
    return vec;
}
Vector4 Vector4::operator- (const Vector4 & other) const
{
    Vector4 vec(x - other.x, y - other.y, z - other.z, w - other.w);
    return vec;
}
Vector4 Lurdr::operator* (float multiplier, const Vector4 & multiplicand)
{
    Vector4 vec(
        multiplier * multiplicand.x,
        multiplier * multiplicand.y,
        multiplier * multiplicand.z,
        multiplier * multiplicand.w
    );
    return vec;
}
Vector4 Vector4::operator* (const float & multiplicand) const
{
    Vector4 vec(x * multiplicand, y * multiplicand, z * multiplicand, w * multiplicand);
    return vec;
}
Vector4 Vector4::operator/ (const float divisor) const
{
    assert(fabs(divisor) > EPSILON);
    float factor = 1.0f / divisor;
    Vector4 vec(x * factor, y * factor, z * factor, w * factor);
    return vec;
}
bool Vector4::operator== (const Vector4 & other) const
{
    if ( fabs(x - other.x) <= EPSILON * max(max(fabs(x), fabs(other.x)), 1.0f)
      && fabs(y - other.y) <= EPSILON * max(max(fabs(y), fabs(other.y)), 1.0f)
      && fabs(z - other.z) <= EPSILON * max(max(fabs(z), fabs(other.z)), 1.0f)
      && fabs(w - other.w) <= EPSILON * max(max(fabs(w), fabs(other.w)), 1.0f) )
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Vector4::operator!= (const Vector4 & other) const
{
    if (*this == other)
    {
        return false;
    }
    else
    {
        return true;
    }
}
void Vector4::operator+= (const Vector4 & other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
}
float Vector4::dot(const Vector4 & other) const
{
    return x * other.x + y * other.y + z * other.z + w * other.w;
}
float Vector4::length() const
{
    return sqrtf(x * x + y * y + z * z + w * w);
}
Vector4 Vector4::normalized() const
{
    float len = length();
    assert(fabs(len) > EPSILON);
    float factor = 1.0f / len;
    Vector4 vec(
        x * factor,
        y * factor,
        z * factor,
        w * factor
    );
    return vec;
}
void Vector4::normalize()
{
    float len = length();
    assert(fabs(len) > EPSILON);
    float factor = 1.0f / len;
    x *= factor;
    y *= factor;
    z *= factor;
    w *= factor;
}
void Vector4::print() const
{
    printf("Vector4 : ( %9.3f, %9.3f, %9.3f, %9.3f )\n", x, y, z, w);
}

Vector4 Vector4::lerp(const Vector4 & from, const Vector4 & to, float alpha)
{
    alpha = clamp(alpha, 0.0f, 1.0f);
    Vector4 vec(
        from.x * (1 - alpha) + to.x * alpha,
        from.y * (1 - alpha) + to.y * alpha,
        from.z * (1 - alpha) + to.z * alpha,
        from.w * (1 - alpha) + to.w * alpha
    );
    return vec;
}

/**
 * Quaternion : functionalities and utilities
 * API reference : https://docs.unity.cn/2019.4/Documentation/ScriptReference/Quaternion.html
 */

Quaternion Quaternion::IDENTITY = Quaternion(0.0, 0.0, 0.0, 1.0);

void Quaternion::operator= (const Quaternion & other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
}
// reference : https://en.wikipedia.org/wiki/Quaternion
// & https://ww2.mathworks.cn/help/aeroblks/quaternionmultiplication.html
// & http://kieranwynn.github.io/pyquaternion/ for cross reference
// note : 'a + bi + cj + dk' <=> 'xi + yj + zk + w'
Quaternion Quaternion::operator* (const Quaternion & other) const
{
    Quaternion quat(
        other.w * x + other.x * w - other.y * z + other.z * y,
        other.w * y + other.x * z + other.y * w - other.z * x,
        other.w * z - other.x * y + other.y * x + other.z * w,
        other.w * w - other.x * x - other.y * y - other.z * z
    );
    return quat;
}
bool Quaternion::operator== (const Quaternion & other) const
{
    if ( fabs(x - other.x) <= EPSILON * max(max(fabs(x), fabs(other.x)), 1.0f)
      && fabs(y - other.y) <= EPSILON * max(max(fabs(y), fabs(other.y)), 1.0f)
      && fabs(z - other.z) <= EPSILON * max(max(fabs(z), fabs(other.z)), 1.0f)
      && fabs(w - other.w) <= EPSILON * max(max(fabs(w), fabs(other.w)), 1.0f) )
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Quaternion::operator!= (const Quaternion & other) const
{
    if (*this == other)
    {
        return false;
    }
    else
    {
        return true;
    }
}
float Quaternion::dot(const Quaternion & other) const
{
    // note : return the real part of multiplication of two quaternions
    // which is ((*this).inversed() * other).w
    return x * other.x + y * other.y + z * other.z + w * other.w;
}
Quaternion Quaternion::normalized() const
{
    float len = sqrtf(dot(*this));
    assert(fabs(len) > EPSILON);
    float factor = 1.0f / len;
    Quaternion quat(
        x * factor,
        y * factor,
        z * factor,
        w * factor
    );
    return quat;
}
void Quaternion::normalize()
{
    float len = sqrtf(dot(*this));
    assert(fabs(len) > EPSILON);
    float factor = 1.0f / len;
    x *= factor;
    y *= factor;
    z *= factor;
    w *= factor;
}
// reference : https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// & https://personal.utdallas.edu/~sxb027100/dock/quaternion.html
Vector3 Quaternion::toEulerAngles() const
{
    float roll = atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
    float p = 2.0f * (w * y - x * z);
    float pitch;
    if (fabs(p) >= 1.0f)
    {
        // handle 90 degree rotation
        pitch = copysign(PI / 2.0f, p);
    }
    else
    {
        pitch = asin(p);
    }
    float yaw = atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));

    Vector3 vec(roll, pitch, yaw);
    return vec;
}
// nonzero quaternion has an inverse with respect to its Hamilton product
Quaternion Quaternion::inversed() const
{
    float q = dot(*this);
    assert(fabs(q) > EPSILON);
    float factor = 1.0f / q;
    Quaternion quat(
        -x * factor,
        -y * factor,
        -z * factor,
         w * factor
    );
    return quat;
}
void Quaternion::print() const
{
    printf("Quaternion : ( %9.3fi + %9.3fj + %9.3fk + %9.3f )\n", x, y, z, w);
}
// linear lerp for quaternion
Quaternion Quaternion::lerp(const Quaternion & from, const Quaternion & to, float alpha)
{
    alpha = clamp(alpha, 0.0f, 1.0f);
    Quaternion quat(
        from.x * (1 - alpha) + to.x * alpha,
        from.y * (1 - alpha) + to.y * alpha,
        from.z * (1 - alpha) + to.z * alpha,
        from.w * (1 - alpha) + to.w * alpha
    );
    return quat;
}
// spherical lerp for quaternion
// reference : https://github.com/zauonlok/renderer/blob/master/renderer/core/maths.c
// & https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
Quaternion Quaternion::slerp(const Quaternion & from, const Quaternion & to, float alpha)
{
    alpha = clamp(alpha, 0.0f, 1.0f);
    float cosa = from.dot(to);
    Quaternion target = to;
    if (cosa < 0)
    {
        // negate the target to lerp in the smaller arc
        target.x = -target.x;
        target.y = -target.y;
        target.z = -target.z;
        target.w = -target.w;
        cosa = -cosa;
    }
    if (cosa > 1.0f - EPSILON)
    {
        // almost parellel, lerp linearly
        Quaternion quat(
            from.x * (1 - alpha) + target.x * alpha,
            from.y * (1 - alpha) + target.y * alpha,
            from.z * (1 - alpha) + target.z * alpha,
            from.w * (1 - alpha) + target.w * alpha
        );
        return quat;
    }
    else
    {
        // reference : https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
        // & https://www.geometrictools.com/Documentation/Quaternions.pdf
        float a = acosf(cosa);
        float sina = sinf(a);
        float angle_from = (1.0f - alpha) * a;
        float angle_target = alpha * a;
        float factor_from = sinf(angle_from) / sina;
        float factor_target = sinf(angle_target) / sina;

        Quaternion quat(
            from.x * factor_from + target.x * factor_target,
            from.y * factor_from + target.y * factor_target,
            from.z * factor_from + target.z * factor_target,
            from.w * factor_from + target.w * factor_target
        );
        return quat;

        // // a method by calculating relative rotation between two quaternions
        // // double the time consuming than the above method
        // float a = acosf(cosa);
        // float new_angle = alpha * a;
        // float factor = sinf(new_angle) / sinf(a);
        // Quaternion q = from.inversed() * target;
        
        // Quaternion quat(
        //     q.x * factor,
        //     q.y * factor,
        //     q.z * factor,
        //     cosf(new_angle)
        // );
        // return from * quat;
    }
}
// reference : https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Quaternion Quaternion::fromEulerAngles(const Vector3 & angles)
{
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
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy
    );
    return quat;
}
// reference : https://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another
Quaternion Quaternion::fromToRotation(const Vector3 & from, const Vector3 to)
{
    Vector3 from_n = from.normalized();
    Vector3 to_n = to.normalized();
    float dot = from_n.dot(to_n);
    if (dot > 1.0f - EPSILON)
    {
        // parallel vectors pointing the same direction
        return IDENTITY;
    }
    else if (dot < -1.0f + EPSILON)
    {
        // parallel vectors pointing opposite direction
        return fromAxisAngle(Vector3::UNIT_X, PI);
    }
    else
    {
        Vector3 cross = from_n.cross(to_n);
        Quaternion quat(
            cross.x,
            cross.y,
            cross.z,
            1.0f + dot
        );
        return quat.normalized();
    }
}
// note : angle is mesured in radiance
Quaternion Quaternion::fromAxisAngle(const Vector3 & axis, float angle)
{
    float cosa = cosf(angle * 0.5f);
    float sina = sinf(angle * 0.5f);
    Quaternion quat(
        sina * axis.x,
        sina * axis.y,
        sina * axis.z,
        cosa
    );
    return quat;
}

/**
 * Matrix3 : functionalities and utilities
 */

Matrix3 Matrix3::IDENTITY = Matrix3( 1.0f, 0.0f, 0.0f, 
                                     0.0f, 1.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f );

Matrix3::Matrix3()
{
    m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f;
    m[3] = 0.0f; m[4] = 1.0f; m[5] = 0.0f;
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 1.0f;
}
Matrix3::Matrix3(
    const float & m1, const float & m2, const float & m3,
    const float & m4, const float & m5, const float & m6,
    const float & m7, const float & m8, const float & m9 )
{
    m[0] = m1; m[1] = m2; m[2] = m3;
    m[3] = m4; m[4] = m5; m[5] = m6;
    m[6] = m7; m[7] = m8; m[8] = m9;
}
Matrix3::Matrix3(const Matrix3 & other)
{
    memcpy(m, other.m, 9 * sizeof(float));
}

void Matrix3::operator= (const Matrix3 & other)
{
    memcpy(m, other.m, 9 * sizeof(float));
}
float& Matrix3::operator() (const size_t & row, const size_t & col)
{
    assert(row >= 0 && row < 3);
    assert(col >= 0 && col < 3);

    return m[row * 3 + col];
}
Matrix3 Matrix3::operator+ (const Matrix3 & other) const
{   
    Matrix3 mat(
        m[0] + other.m[0], m[1] + other.m[1], m[2] + other.m[2],
        m[3] + other.m[3], m[4] + other.m[4], m[5] + other.m[5],
        m[6] + other.m[6], m[7] + other.m[7], m[8] + other.m[8]
    );
    return mat;
}
Matrix3 Matrix3::operator- (const Matrix3 & other) const
{
    Matrix3 mat(
        m[0] - other.m[0], m[1] - other.m[1], m[2] - other.m[2],
        m[3] - other.m[3], m[4] - other.m[4], m[5] - other.m[5],
        m[6] - other.m[6], m[7] - other.m[7], m[8] - other.m[8]
    );
    return mat;
}
Matrix3 Matrix3::operator* (const Matrix3 & other) const
{
    Matrix3 mat(
        m[0] * other.m[0] + m[1] * other.m[3] + m[2] * other.m[6],
        m[0] * other.m[1] + m[1] * other.m[4] + m[2] * other.m[7],
        m[0] * other.m[2] + m[1] * other.m[5] + m[2] * other.m[8],

        m[3] * other.m[0] + m[4] * other.m[3] + m[5] * other.m[6],
        m[3] * other.m[1] + m[4] * other.m[4] + m[5] * other.m[7],
        m[3] * other.m[2] + m[4] * other.m[5] + m[5] * other.m[8],

        m[6] * other.m[0] + m[7] * other.m[3] + m[8] * other.m[6],
        m[6] * other.m[1] + m[7] * other.m[4] + m[8] * other.m[7],
        m[6] * other.m[2] + m[7] * other.m[5] + m[8] * other.m[8]
    );
    return mat;
}
Vector3 Matrix3::operator* (const Vector3 & other) const
{
    Vector3 vec(
        m[0] * other.x + m[1] * other.y + m[2] * other.z,
        m[3] * other.x + m[4] * other.y + m[5] * other.z,
        m[6] * other.x + m[7] * other.y + m[8] * other.z
    );
    return vec;
}
Matrix3 Matrix3::operator/ (const float & divisor) const
{
    assert(fabs(divisor) > EPSILON);
    float factor = 1.0f / divisor;
    Matrix3 mat(
        m[0] * factor, m[1] * factor, m[2] * factor,
        m[3] * factor, m[4] * factor, m[5] * factor,
        m[6] * factor, m[7] * factor, m[8] * factor
    );
    return mat;
}
bool Matrix3::operator== (const Matrix3 & other) const
{
    if ( fabs(m[0] - other.m[0]) <= EPSILON * max(max(fabs(m[0]), fabs(other.m[0])), 1.0f)
      && fabs(m[1] - other.m[1]) <= EPSILON * max(max(fabs(m[1]), fabs(other.m[1])), 1.0f)
      && fabs(m[2] - other.m[2]) <= EPSILON * max(max(fabs(m[2]), fabs(other.m[2])), 1.0f)
      && fabs(m[3] - other.m[3]) <= EPSILON * max(max(fabs(m[3]), fabs(other.m[3])), 1.0f) 
      && fabs(m[4] - other.m[4]) <= EPSILON * max(max(fabs(m[4]), fabs(other.m[4])), 1.0f) 
      && fabs(m[5] - other.m[5]) <= EPSILON * max(max(fabs(m[5]), fabs(other.m[5])), 1.0f) 
      && fabs(m[6] - other.m[6]) <= EPSILON * max(max(fabs(m[6]), fabs(other.m[6])), 1.0f) 
      && fabs(m[7] - other.m[7]) <= EPSILON * max(max(fabs(m[7]), fabs(other.m[7])), 1.0f) 
      && fabs(m[8] - other.m[8]) <= EPSILON * max(max(fabs(m[8]), fabs(other.m[8])), 1.0f) )
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Matrix3::operator!= (const Matrix3 & other) const
{
    if (*this == other)
    {
        return false;
    }
    else
    {
        return true;
    }
}
// hadamard product : https://en.wikipedia.org/wiki/Hadamard_product_(matrices)
Matrix3 Matrix3::hadamard(const Matrix3 & other) const
{
    Matrix3 mat(
        m[0] * other.m[0], m[1] * other.m[1], m[2] * other.m[2],
        m[3] * other.m[3], m[4] * other.m[4], m[5] * other.m[5],
        m[6] * other.m[6], m[7] * other.m[7], m[8] * other.m[8]
    );
    return mat;
}
Matrix3 Matrix3::inversed() const
{
    float det = (*this).det();
    assert(fabs(det) > EPSILON);

    float factor = 1.0f / det;
    float a1 = m[4] * m[8] - m[5] * m[7];
    float a2 = m[3] * m[8] - m[5] * m[6];
    float a3 = m[3] * m[7] - m[4] * m[6];
    float a4 = m[1] * m[8] - m[2] * m[7];
    float a5 = m[0] * m[8] - m[2] * m[6];
    float a6 = m[0] * m[7] - m[1] * m[6];
    float a7 = m[1] * m[5] - m[2] * m[4];
    float a8 = m[0] * m[5] - m[2] * m[3];
    float a9 = m[0] * m[4] - m[1] * m[3];

    Matrix3 mat(
         a1 * factor, -a4 * factor,  a7 * factor,
        -a2 * factor,  a5 * factor, -a8 * factor,
         a3 * factor, -a6 * factor,  a9 * factor
    );
    return mat;
}
float Matrix3::det() const
{
    return m[0] * (m[4] * m[8] - m[5] * m[7])
         - m[1] * (m[3] * m[8] - m[5] * m[6])
         + m[2] * (m[3] * m[7] - m[4] * m[6]);
}
Matrix3 Matrix3::transposed() const
{
    Matrix3 mat(
        m[0], m[3], m[6],
        m[1], m[4], m[7],
        m[2], m[5], m[8]
    );
    return mat;
}
void Matrix3::print() const
{
    printf("Matrix3 : ( %9.3f, %9.3f, %9.3f, \n", m[0], m[1], m[2]);
    printf("            %9.3f, %9.3f, %9.3f, \n", m[3], m[4], m[5]);
    printf("            %9.3f, %9.3f, %9.3f )\n", m[6], m[7], m[8]);
}

/**
 * Matrix4 : functionalities and utilities
 */

Matrix4 Matrix4::IDENTITY = Matrix4(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
);

Matrix4::Matrix4()
{
    m[0]  = 1.0f; m[1]  = 0.0f; m[2]  = 0.0f; m[3]  = 0.0f; 
    m[4]  = 0.0f; m[5]  = 1.0f; m[6]  = 0.0f; m[7]  = 0.0f; 
    m[8]  = 0.0f; m[9]  = 0.0f; m[10] = 1.0f; m[11] = 0.0f; 
    m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f; 
}
Matrix4::Matrix4(
    const float & m1,  const float & m2,  const float & m3,  const float & m4,
    const float & m5,  const float & m6,  const float & m7,  const float & m8,
    const float & m9,  const float & m10, const float & m11, const float & m12,
    const float & m13, const float & m14, const float & m15, const float & m16 )
{
    m[0]  = m1;  m[1]  = m2;  m[2]  = m3;  m[3]  = m4; 
    m[4]  = m5;  m[5]  = m6;  m[6]  = m7;  m[7]  = m8; 
    m[8]  = m9;  m[9]  = m10; m[10] = m11; m[11] = m12; 
    m[12] = m13; m[13] = m14; m[14] = m15; m[15] = m16; 
}
Matrix4::Matrix4(const Matrix4 & other)
{
    memcpy(m, other.m, 16 * sizeof(float));
}
void Matrix4::operator= (const Matrix4 & other)
{
    memcpy(m, other.m, 16 * sizeof(float));
}
float& Matrix4::operator() (const size_t & row, const size_t & col)
{
    assert(row >= 0 && row < 4);
    assert(col >= 0 && col < 4);
    return m[row * 4 + col];
}
Matrix4 Matrix4::operator+ (const Matrix4 & other) const
{
    Matrix4 mat(
        m[0]  + other.m[0],  m[1]  + other.m[1],  m[2]  + other.m[2],  m[3] + other.m[3],
        m[4]  + other.m[4],  m[5]  + other.m[5],  m[6]  + other.m[6],  m[3] + other.m[7],
        m[8]  + other.m[8],  m[9]  + other.m[9],  m[10] + other.m[10], m[3] + other.m[11],
        m[12] + other.m[12], m[13] + other.m[13], m[14] + other.m[14], m[3] + other.m[15]
    );
    return mat;
}
Matrix4 Matrix4::operator- (const Matrix4 & other) const
{
    Matrix4 mat(
        m[0]  - other.m[0],  m[1]  - other.m[1],  m[2]  - other.m[2],  m[3] - other.m[3],
        m[4]  - other.m[4],  m[5]  - other.m[5],  m[6]  - other.m[6],  m[3] - other.m[7],
        m[8]  - other.m[8],  m[9]  - other.m[9],  m[10] - other.m[10], m[3] - other.m[11],
        m[12] - other.m[12], m[13] - other.m[13], m[14] - other.m[14], m[3] - other.m[15]
    );
    return mat;
}
Matrix4 Matrix4::operator* (const Matrix4 & other) const
{
    Matrix4 mat(
        m[0] * other.m[0] + m[1] * other.m[4] + m[2] * other.m[8]  + m[3] * other.m[12],
        m[0] * other.m[1] + m[1] * other.m[5] + m[2] * other.m[9]  + m[3] * other.m[13],
        m[0] * other.m[2] + m[1] * other.m[6] + m[2] * other.m[10] + m[3] * other.m[14],
        m[0] * other.m[3] + m[1] * other.m[7] + m[2] * other.m[11] + m[3] * other.m[15],

        m[4] * other.m[0] + m[5] * other.m[4] + m[6] * other.m[8]  + m[7] * other.m[12],
        m[4] * other.m[1] + m[5] * other.m[5] + m[6] * other.m[9]  + m[7] * other.m[13],
        m[4] * other.m[2] + m[5] * other.m[6] + m[6] * other.m[10] + m[7] * other.m[14],
        m[4] * other.m[3] + m[5] * other.m[7] + m[6] * other.m[11] + m[7] * other.m[15],

        m[8] * other.m[0] + m[9] * other.m[4] + m[10] * other.m[8]  + m[11] * other.m[12],
        m[8] * other.m[1] + m[9] * other.m[5] + m[10] * other.m[9]  + m[11] * other.m[13],
        m[8] * other.m[2] + m[9] * other.m[6] + m[10] * other.m[10] + m[11] * other.m[14],
        m[8] * other.m[3] + m[9] * other.m[7] + m[10] * other.m[11] + m[11] * other.m[15],

        m[12] * other.m[0] + m[13] * other.m[4] + m[14] * other.m[8]  + m[15] * other.m[12],
        m[12] * other.m[1] + m[13] * other.m[5] + m[14] * other.m[9]  + m[15] * other.m[13],
        m[12] * other.m[2] + m[13] * other.m[6] + m[14] * other.m[10] + m[15] * other.m[14],
        m[12] * other.m[3] + m[13] * other.m[7] + m[14] * other.m[11] + m[15] * other.m[15]
    );
    return mat;
}
Vector4 Matrix4::operator* (const Vector4 & other) const
{
    Vector4 vec(
        m[0]  * other.x + m[1]  * other.y + m[2]  * other.z + m[3]  * other.w,
        m[4]  * other.x + m[5]  * other.y + m[6]  * other.z + m[7]  * other.w,
        m[8]  * other.x + m[9]  * other.y + m[10] * other.z + m[11] * other.w,
        m[12] * other.x + m[13] * other.y + m[14] * other.z + m[15] * other.w
    );
    return vec;
}
Matrix4 Matrix4::operator/ (const float & divisor) const
{
    assert(fabs(divisor) > EPSILON);
    float factor = 1.0f / divisor;
    Matrix4 mat(
        m[0]  * factor, m[1]  * factor, m[2]  * factor, m[3] * factor,
        m[4]  * factor, m[5]  * factor, m[6]  * factor, m[3] * factor,
        m[8]  * factor, m[9]  * factor, m[10] * factor, m[3] * factor,
        m[12] * factor, m[13] * factor, m[14] * factor, m[3] * factor
    );
    return mat;
}
bool Matrix4::operator== (const Matrix4 & other) const
{
    if ( fabs(m[0] - other.m[0]) <= EPSILON * max(max(fabs(m[0]), fabs(other.m[0])), 1.0f)
      && fabs(m[1] - other.m[1]) <= EPSILON * max(max(fabs(m[1]), fabs(other.m[1])), 1.0f)
      && fabs(m[2] - other.m[2]) <= EPSILON * max(max(fabs(m[2]), fabs(other.m[2])), 1.0f)
      && fabs(m[3] - other.m[3]) <= EPSILON * max(max(fabs(m[3]), fabs(other.m[3])), 1.0f) 
      && fabs(m[4] - other.m[4]) <= EPSILON * max(max(fabs(m[4]), fabs(other.m[4])), 1.0f) 
      && fabs(m[5] - other.m[5]) <= EPSILON * max(max(fabs(m[5]), fabs(other.m[5])), 1.0f) 
      && fabs(m[6] - other.m[6]) <= EPSILON * max(max(fabs(m[6]), fabs(other.m[6])), 1.0f) 
      && fabs(m[7] - other.m[7]) <= EPSILON * max(max(fabs(m[7]), fabs(other.m[7])), 1.0f) 
      && fabs(m[8] - other.m[8]) <= EPSILON * max(max(fabs(m[8]), fabs(other.m[8])), 1.0f)
      && fabs(m[9] - other.m[9]) <= EPSILON * max(max(fabs(m[9]), fabs(other.m[9])), 1.0f)
      && fabs(m[10] - other.m[10]) <= EPSILON * max(max(fabs(m[10]), fabs(other.m[10])), 1.0f)
      && fabs(m[11] - other.m[11]) <= EPSILON * max(max(fabs(m[11]), fabs(other.m[11])), 1.0f)
      && fabs(m[12] - other.m[12]) <= EPSILON * max(max(fabs(m[12]), fabs(other.m[12])), 1.0f) 
      && fabs(m[13] - other.m[13]) <= EPSILON * max(max(fabs(m[13]), fabs(other.m[13])), 1.0f) 
      && fabs(m[14] - other.m[14]) <= EPSILON * max(max(fabs(m[14]), fabs(other.m[14])), 1.0f) 
      && fabs(m[15] - other.m[15]) <= EPSILON * max(max(fabs(m[15]), fabs(other.m[15])), 1.0f) )
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Matrix4::operator!= (const Matrix4 & other) const
{
    if (*this == other)
    {
        return false;
    }
    else
    {
        return true;
    }
}

Matrix4 Matrix4::hadamard(const Matrix4 & other) const
{
    Matrix4 mat(
        m[0]  * other.m[0],  m[1]  * other.m[1],  m[2]  * other.m[2],  m[3] * other.m[3],
        m[4]  * other.m[4],  m[5]  * other.m[5],  m[6]  * other.m[6],  m[3] * other.m[7],
        m[8]  * other.m[8],  m[9]  * other.m[9],  m[10] * other.m[10], m[3] * other.m[11],
        m[12] * other.m[12], m[13] * other.m[13], m[14] * other.m[14], m[3] * other.m[15]
    );
    return mat;
}
Matrix4 Matrix4::inversed() const
{
    float det = (*this).det();
    assert(fabs(det) > EPSILON);
    float factor = 1.0f / det;
    float m11 = Matrix3(m[5],  m[6],  m[7], 
                        m[9],  m[10], m[11], 
                        m[13], m[14], m[15]).det();
    float m12 = Matrix3(m[4],  m[6],  m[7], 
                        m[8],  m[10], m[11], 
                        m[12], m[14], m[15]).det();
    float m13 = Matrix3(m[4],  m[5],  m[7], 
                        m[8],  m[9],  m[11], 
                        m[12], m[13], m[15]).det();
    float m14 = Matrix3(m[4],  m[5],  m[6], 
                        m[8],  m[9],  m[10], 
                        m[12], m[13], m[14]).det();
    float m21 = Matrix3(m[1],  m[2],  m[3], 
                        m[9],  m[10], m[11], 
                        m[13], m[14], m[15]).det();
    float m22 = Matrix3(m[0],  m[2],  m[3], 
                        m[8],  m[10], m[11], 
                        m[12], m[14], m[15]).det();
    float m23 = Matrix3(m[0],  m[1],  m[3], 
                        m[8],  m[9],  m[11], 
                        m[12], m[13], m[15]).det();
    float m24 = Matrix3(m[0],  m[1],  m[2], 
                        m[8],  m[9],  m[10], 
                        m[12], m[13], m[14]).det();
    float m31 = Matrix3(m[1],  m[2],  m[3], 
                        m[5],  m[6],  m[7], 
                        m[13], m[14], m[15]).det();
    float m32 = Matrix3(m[0],  m[2],  m[3], 
                        m[4],  m[6],  m[7], 
                        m[12], m[14], m[15]).det();
    float m33 = Matrix3(m[0],  m[1],  m[3], 
                        m[4],  m[5],  m[7], 
                        m[12], m[13], m[15]).det();
    float m34 = Matrix3(m[0],  m[1],  m[2], 
                        m[4],  m[5],  m[6], 
                        m[12], m[13], m[14]).det();
    float m41 = Matrix3(m[1],  m[2],  m[3], 
                        m[5],  m[6],  m[7], 
                        m[9],  m[10], m[11]).det();
    float m42 = Matrix3(m[0],  m[2],  m[3], 
                        m[4],  m[6],  m[7], 
                        m[8],  m[10], m[11]).det();
    float m43 = Matrix3(m[0],  m[1],  m[3], 
                        m[4],  m[5],  m[7], 
                        m[8],  m[9],  m[11]).det();
    float m44 = Matrix3(m[0],  m[1],  m[2], 
                        m[4],  m[5],  m[6], 
                        m[8],  m[9],  m[10]).det();
    Matrix4 mat(
         m11 * factor, -m21 * factor,  m31 * factor, -m41 * factor, 
        -m12 * factor,  m22 * factor, -m32 * factor,  m42 * factor, 
         m13 * factor, -m23 * factor,  m33 * factor, -m43 * factor, 
        -m14 * factor,  m24 * factor, -m34 * factor,  m44 * factor
    );
    return mat;
}
float Matrix4::det() const
{
    return 
        m[0] * Matrix3(m[5],  m[6],  m[7], 
                       m[9],  m[10], m[11], 
                       m[13], m[14], m[15]).det()
      - m[1] * Matrix3(m[4],  m[6],  m[7], 
                       m[8],  m[10], m[11], 
                       m[12], m[14], m[15]).det()
      + m[2] * Matrix3(m[4],  m[5],  m[7], 
                       m[8],  m[9],  m[11], 
                       m[12], m[13], m[15]).det()
      - m[3] * Matrix3(m[4],  m[5],  m[6], 
                       m[8],  m[9],  m[10], 
                       m[12], m[13], m[14]).det();
}
Matrix4 Matrix4::transposed() const
{
    Matrix4 mat(
        m[0], m[4], m[8],  m[12],
        m[1], m[5], m[9],  m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    );
    return mat;
}
void Matrix4::print() const
{
    printf("Matrix4 : ( %9.3f, %9.3f, %9.3f, %9.3f, \n", m[0],  m[1],  m[2],  m[3]);
    printf("            %9.3f, %9.3f, %9.3f, %9.3f, \n", m[4],  m[5],  m[6],  m[7]);
    printf("            %9.3f, %9.3f, %9.3f, %9.3f, \n", m[8],  m[9],  m[10], m[11]);
    printf("            %9.3f, %9.3f, %9.3f, %9.3f )\n", m[12], m[13], m[14], m[15]);
}
void Matrix4::multiply(const Matrix4 & other)
{
    m[0] = m[0] * other.m[0] + m[1] * other.m[4] + m[2] * other.m[8]  + m[3] * other.m[12];
    m[1] = m[0] * other.m[1] + m[1] * other.m[5] + m[2] * other.m[9]  + m[3] * other.m[13];
    m[2] = m[0] * other.m[2] + m[1] * other.m[6] + m[2] * other.m[10] + m[3] * other.m[14];
    m[3] = m[0] * other.m[3] + m[1] * other.m[7] + m[2] * other.m[11] + m[3] * other.m[15];

    m[4] = m[4] * other.m[0] + m[5] * other.m[4] + m[6] * other.m[8]  + m[7] * other.m[12];
    m[5] = m[4] * other.m[1] + m[5] * other.m[5] + m[6] * other.m[9]  + m[7] * other.m[13];
    m[6] = m[4] * other.m[2] + m[5] * other.m[6] + m[6] * other.m[10] + m[7] * other.m[14];
    m[7] = m[4] * other.m[3] + m[5] * other.m[7] + m[6] * other.m[11] + m[7] * other.m[15];

    m[8] = m[8] * other.m[0] + m[9] * other.m[4] + m[10] * other.m[8]  + m[11] * other.m[12];
    m[9] = m[8] * other.m[1] + m[9] * other.m[5] + m[10] * other.m[9]  + m[11] * other.m[13];
    m[10] = m[8] * other.m[2] + m[9] * other.m[6] + m[10] * other.m[10] + m[11] * other.m[14];
    m[11] = m[8] * other.m[3] + m[9] * other.m[7] + m[10] * other.m[11] + m[11] * other.m[15];

    m[12] = m[12] * other.m[0] + m[13] * other.m[4] + m[14] * other.m[8]  + m[15] * other.m[12];
    m[13] = m[12] * other.m[1] + m[13] * other.m[5] + m[14] * other.m[9]  + m[15] * other.m[13];
    m[14] = m[12] * other.m[2] + m[13] * other.m[6] + m[14] * other.m[10] + m[15] * other.m[14];
    m[15] = m[12] * other.m[3] + m[13] * other.m[7] + m[14] * other.m[11] + m[15] * other.m[15];
}
Matrix4 Matrix4::fromQuaternion(const Quaternion & quat)
{
    return fromTRS(vec3::ZERO, quat, vec3(1.0f, 1.0f, 1.0f));
}
// reference : http://www.songho.ca/opengl/gl_quaternion.html
Matrix4 Matrix4::fromTRS(const Vector3 & translation, const Quaternion & rotation, const Vector3 & scale)
{
    assert(fabs(scale.x) > EPSILON);
    assert(fabs(scale.y) > EPSILON);
    assert(fabs(scale.z) > EPSILON);

    float xx = rotation.x * rotation.x;
    float xy = rotation.x * rotation.y;
    float xz = rotation.x * rotation.z;
    float xw = rotation.x * rotation.w;
    float yy = rotation.y * rotation.y;
    float yz = rotation.y * rotation.z;
    float yw = rotation.y * rotation.w;
    float zz = rotation.z * rotation.z;
    float zw = rotation.z * rotation.w;

    float m11 = (1.0f - 2.0f * yy - 2.0f * zz) * scale.x;
    float m22 = (1.0f - 2.0f * xx - 2.0f * zz) * scale.y;
    float m33 = (1.0f - 2.0f * xx - 2.0f * yy) * scale.z;

    Matrix4 mat(
        m11,                    2.0f * xy - 2.0f * zw,  2.0f * xz + 2.0f * yw,  translation.x,
        2.0f * xy + 2.0f * zw,  m22,                    2.0f * yz - 2.0f * xw,  translation.y,
        2.0f * xz - 2.0f * yw,  2.0f * yz + 2.0f * xw,  m33,                    translation.z,
        0.0f,                   0.0f,                   0.0f,                   1.0f
    );
    return mat;
}
// reference : https://en.wikipedia.org/wiki/Rotation_matrix
Matrix4 Matrix4::fromAxisAngle(const Vector3 & axis, const float & angle)
{
    assert(fabs(axis.x) > EPSILON);
    assert(fabs(axis.y) > EPSILON);
    assert(fabs(axis.z) > EPSILON);

    float sin = sinf(angle);
    float cos = cosf(angle);
    float xx = axis.x * axis.x;
    float xy = axis.x * axis.y;
    float xz = axis.x * axis.z;
    float yy = axis.y * axis.y;
    float yz = axis.y * axis.z;
    float zz = axis.z * axis.z;

    Matrix4 mat(
        cos + xx * (1.0f - cos), xy * (1.0f - cos) - axis.z * sin, xz * (1.0f - cos) + axis.y * sin, 0.0f,
        xy * (1.0f - cos) + axis.z * sin, cos + yy * (1.0f - cos), yz * (1.0f - cos) - axis.x * sin, 0.0f,
        xz * (1.0f - cos) - axis.y * sin, yz * (1.0f - cos) + axis.x * sin, cos + zz * (1.0f - cos), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f 
    );
    return mat;
}
// reference : http://www.songho.ca/opengl/gl_camera.html
Matrix4 Matrix4::fromLookAt(const Vector3 & eye, const Vector3 & target, const Vector3 & up)
{
    Vector3 forward = (eye - target).normalized();
    Vector3 left = up.cross(forward).normalized();
    Vector3 upward = forward.cross(left); // cross product of two normalized vector is also normalized
    Matrix4 mat(
        left.x,     left.y,     left.z,     -(left.x * eye.x + left.y * eye.y + left.z * eye.z),
        upward.x,   upward.y,   upward.z,   -(upward.x * eye.x + upward.y * eye.y + upward.z * eye.z),
        forward.x,  forward.y,  forward.z,  -(forward.x * eye.x + forward.y * eye.y + forward.z * eye.z),
        0.0f,       0.0f,       0.0f,       1.0f
    );
    return mat;
}

// reference : http://www.songho.ca/opengl/gl_projectionmatrix.html
Matrix4 Matrix4::getProjection(const float & fov, const float & aspect, const float & near, const float & far)
{
    float z_range = far - near;
    assert(fov > 0 && aspect > 0);
    assert(near > 0 && far > 0 && z_range > 0);

    Matrix4 mat(
        1.0f / (aspect * tanf(fov / 2.0f)), 0.0f,                    0.0f,                    0.0f,
        0.0f,                               1.0f / tanf(fov / 2.0f), 0.0f,                    0.0f,
        0.0f,                               0.0f,                    -(near + far) / z_range, -2.0 * far * near / z_range,
        0.0f,                               0.0f,                    -1.0f,                   0.0f
    );
    return mat;
}

Matrix4 Matrix4::translated(const Vector3 & translation) const
{
    Matrix4 mat(
        m[0],  m[1],  m[2],  m[3]  + translation.x * m[0] + translation.y * m[1] + translation.z * m[2],
        m[4],  m[5],  m[6],  m[7]  + translation.x * m[4] + translation.y * m[5] + translation.z * m[6],
        m[8],  m[9],  m[10], m[11] + translation.z * m[8] + translation.y * m[9] + translation.z * m[10],
        m[12], m[13], m[14], m[15]
    );
    return mat;
}
Matrix4 Matrix4::scaled(const Vector3 & scale) const
{
    assert(fabs(scale.x) > EPSILON);
    assert(fabs(scale.y) > EPSILON);
    assert(fabs(scale.z) > EPSILON);

    Matrix4 mat(
        m[0] * scale.x, m[1] * scale.y, m[2] * scale.z,  m[3],
        m[4] * scale.x, m[5] * scale.y, m[6] * scale.z,  m[7],
        m[8] * scale.x, m[9] * scale.y, m[10] * scale.z, m[11],
        m[12],          m[13],          m[14],           m[15]
    );
    return mat;
}
Matrix4 Matrix4::rotated(const Vector3 & axis, const float & angle) const
{
    Matrix4 mat = fromAxisAngle(axis, angle);
    return (*this) * mat;
}
Matrix4 Matrix4::rotated(const Quaternion & rotation) const
{
    Matrix4 mat = fromQuaternion(rotation);
    return (*this) * mat;
}

void Matrix4::translate(const Vector3 & translation)
{
    m[3]  += translation.x * m[0] + translation.y * m[1] + translation.z * m[2];
    m[7]  += translation.x * m[4] + translation.y * m[5] + translation.z * m[6];
    m[11] += translation.z * m[8] + translation.y * m[9] + translation.z * m[10];
}
void Matrix4::scale(const Vector3 & scale)
{
    assert(fabs(scale.x) > EPSILON);
    assert(fabs(scale.y) > EPSILON);
    assert(fabs(scale.z) > EPSILON);

    m[0]  *= scale.x;
    m[1]  *= scale.y;
    m[2]  *= scale.z;
    m[4]  *= scale.x;
    m[5]  *= scale.y;
    m[6]  *= scale.z;
    m[8]  *= scale.x;
    m[9]  *= scale.y;
    m[10] *= scale.z;
}
void Matrix4::rotate(const Vector3 & axis, const float & angle)
{
    (*this).multiply(fromAxisAngle(axis, angle));
}
void Matrix4::rotate(const Quaternion & rotation)
{
    (*this).multiply(fromQuaternion(rotation));
}