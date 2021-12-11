#include "../include/lmath.h"

using namespace Lurdr;

/**
 * Vector3 Functionalities and Utilities
 */

Vector3 Vector3::UNIT_X = Vector3(1.0, 0.0, 0.0);
Vector3 Vector3::UNIT_Y = Vector3(0.0, 1.0, 0.0);
Vector3 Vector3::UNIT_Z = Vector3(0.0, 0.0, 1.0);
Vector3 Vector3::ZERO   = Vector3(0.0, 0.0, 0.0);

Vector3 Vector3::operator+ (const Vector3 & other) {
    Vector3 vec(x + other.x, y + other.y, z + other.z);
    return vec;
}
Vector3 Vector3::operator- () {
    Vector3 vec(-x, -y, -z);
    return vec;
}
Vector3 Vector3::operator- (const Vector3 & other) {
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
Vector3 Vector3::operator* (const float & multiplicand) {
    Vector3 vec(x * multiplicand, y * multiplicand, z * multiplicand);
    return vec;
}
Vector3 Vector3::operator/ (const float divisor) {
    assert(divisor != 0);
    Vector3 vec(x / divisor, y / divisor, z / divisor);
    return vec;
}
float Vector3::dot(const Vector3 & other) {
    return x * other.x + y * other.y + z * other.z;
}
Vector3 Vector3::cross(const Vector3 & other) {
    Vector3 vec(
        y * other.z - z * other.y,
        x * other.z - z * other.x,
        x * other.y - y * other.z );
    return vec;
}
Vector3 Vector3::lerp(const Vector3 & from, const Vector3 & to, float alpha) {
    alpha = clamp(alpha, 0.0, 1.0);
    Vector3 vec(
        from.x * (1 - alpha) + to.x * alpha,
        from.y * (1 - alpha) + to.y * alpha,
        from.z * (1 - alpha) + to.z * alpha
    );
    return vec;
}

/**
 * Vector2 Functionalities and Utilities
 */

Vector2 Vector2::UNIT_X = Vector2(1.0, 0.0);
Vector2 Vector2::UNIT_Y = Vector2(0.0, 1.0);
Vector2 Vector2::ZERO   = Vector2(0.0, 0.0);

Vector2 Vector2::operator+ (const Vector2 & other) {
    Vector2 vec(x + other.x, y + other.y);
    return vec;
}
Vector2 Vector2::operator- () {
    Vector2 vec(-x, -y);
    return vec;
}
Vector2 Vector2::operator- (const Vector2 & other) {
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
Vector2 Vector2::operator* (const float & multiplicand) {
    Vector2 vec(x * multiplicand, y * multiplicand);
    return vec;
}
Vector2 Vector2::operator/ (const float divisor) {
    assert(divisor != 0);
    Vector2 vec(x / divisor, y / divisor);
    return vec;
}
float Vector2::dot(const Vector2 & other) {
    return x * other.x + y * other.y;
}
Vector2 Vector2::cross(const Vector2 & other) {
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

