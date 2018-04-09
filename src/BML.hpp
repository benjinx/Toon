#ifndef BML_H
#define BML_H

#include <cmath>
#include <iostream>

namespace BML
{

#define PI 3.14159
#define E 2.71828

// Multiply by Degree to get Radian
#define DEG2RAD(X) ((X) * (PI * 2) / 360)

// Multiply by Radian to get Degree
#define RAD2DEG(X) ((X)*360 / (PI * 2))

// Contains a 3 dimensional vector
class vec3
{
public:
    union {
        struct
        {
            float x;
            float y;
            float z;
        };
        float data[3];
    };

    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z = 0.0) : x(x), y(y), z(z) {}

    // Static functions

    // Returns the cross product of two vectors.
    static vec3 cross(vec3 v1, vec3 v2)
    {
        return vec3((v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x));
    }

    // Returns the distance between a and b.
    static float distance(vec3 a, vec3 b)
    {
        return sqrt((b.x - a.x * b.x - a.x) + (b.y - a.y * b.y - a.y) + (b.z - a.z) * (b.z - a.z));
    }

    // Returns the dot product of two vectors.
    static float dot(vec3 v1, vec3 v2) { return float(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z); }

    // Linearly interpolates between two vectors, t is clamped by the range [0, 1].
    static vec3 lerp(vec3 a, vec3 b, float t)
    {
        return vec3((1.0f - t) * a.x + t * b.x, (1.0f - t) * a.y + t * b.y, (1.0f - t) * a.z + t * b.z);
    }

    // Returns the magnitude between two vectors.
    static float magnitude(vec3 v1, vec3 v2)
    {
        return float(sqrt(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z));

        // Bakhshali approximation
        /*
            need to find a way to get the nearest perfect square...
            To calculate sqrt(c^2), let n^2 be the nearest perfect square to c^2
            d = (c^2) - n^2
            p = d / (2n)
            a = n + p
            sqrt(c^2) ~ A - (P^2/2A)
        */
    }

    // Returns the squared magnitude between two vectors.
    static float squaredMagnitude(vec3 v1, vec3 v2) { return float(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z); }

    // Returns a vector that is made from the largest components of two vectors.
    static vec3 max(vec3 lhs, vec3 rhs)
    {
        vec3 returnVec;
        if (lhs.x > rhs.x)
            returnVec.x = lhs.x;
        else if (rhs.x > lhs.x)
            returnVec.x = rhs.x;

        if (lhs.y > rhs.y)
            returnVec.y = lhs.y;
        else if (rhs.y > lhs.y)
            returnVec.y = rhs.y;

        if (lhs.z > rhs.z)
            returnVec.z = lhs.z;
        else if (rhs.z > lhs.z)
            returnVec.z = rhs.z;

        return returnVec;
    }

    // Returns a vector that is made from the smallest components of two vectors.
    static vec3 min(vec3 lhs, vec3 rhs)
    {
        vec3 returnVec;
        if (lhs.x < rhs.x)
            returnVec.x = lhs.x;
        else if (rhs.x < lhs.x)
            returnVec.x = rhs.x;

        if (lhs.y < rhs.y)
            returnVec.y = lhs.y;
        else if (rhs.y < lhs.y)
            returnVec.y = rhs.y;

        if (lhs.z < rhs.z)
            returnVec.z = lhs.z;
        else if (rhs.z < lhs.z)
            returnVec.z = rhs.z;

        return returnVec;
    }

    // Projects vector to, onto vector on and returns the vector.
    static vec3 project(vec3 to, vec3 on)
    {
        float dotProd = dot(to, on);

        float onMag = on.squaredMagnitude();

        vec3 proj;

        proj.x = (dotProd / onMag) * on.x;
        proj.y = (dotProd / onMag) * on.y;
        proj.z = (dotProd / onMag) * on.z;

        return proj;
    }

    // Multiples two vectors component-wise.
    static vec3 scale(vec3 v1, vec3 v2) { return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }

    // Multiples a vector by a given amount.
    static vec3 scale(vec3 v, float amount) { return vec3(v.x * amount, v.y * amount, v.z * amount); }

    // Spherically interpolates between two vectors. These vectors are treated as directions rather than points in
    // space.
    static vec3 slerp(vec3 v1, vec3 v2, float t)
    {
        // http://www.willperone.net/Code/quaternion.php
        // https://en.wikipedia.org/wiki/Slerp

        vec3  slerp;
        float cosOmega = dot(v1, v2);

        if (cosOmega < 0)
        {
            cosOmega = -cosOmega;
            slerp.x  = -v2.x;
            slerp.y  = -v2.y;
            slerp.z  = -v2.z;
        }
        else
        {
            slerp = v2;
        }

        if (cosOmega < 0.95)
        {
            float omega = acos(cosOmega);
            // slerp.x = (sin((1 - t) * omega) / sin(omega)) * v1.x + (sin(t * omega) / sin(omega)) * v2.x;
            // slerp.y = (sin((1 - t) * omega) / sin(omega)) * v1.y + (sin(t * omega) / sin(omega)) * v2.y;
            // slerp.z = (sin((1 - t) * omega) / sin(omega)) * v1.z + (sin(t * omega) / sin(omega)) * v2.z;
            slerp.x = ((sin((1 - t) * omega) * v1.x) + (sin(omega * t) * v2.x)) / sin(omega);
            slerp.x = ((sin((1 - t) * omega) * v1.y) + (sin(omega * t) * v2.y)) / sin(omega);
            slerp.x = ((sin((1 - t) * omega) * v1.z) + (sin(omega * t) * v2.z)) / sin(omega);
        }
        else
        {
            slerp = lerp(v1, v2, t);
        }

        return slerp;
    }

    // Non-static functions

    /* Makes this vector have a magnitude of 1.
    When normalized, a vector keeps the same direction but its length is 1.0.
    Note that this function will change the current vector. If you want to keep the current vector unchanged, use
    normalized variable.
    If this vector is too small to be normalized it will be set to zero.*/
    void normalize()
    {
        float l = magnitude();
        if (l > 0)
        {
            x /= l;
            y /= l;
            z /= l;
        }
    };

    /* Returns a vector with a magnitude of 1.
    When normalized, a vector keeps the same direction but its length is 1.0.
    Note that the current vector is unchanged and a new normalized vector is returned. If you want to normalize the
    current vector, use normalize function.
    If this vector is too small to be normalized it will be set to zero.*/
    vec3 normalized()
    {
        float l = magnitude();
        if (l > 0)
        {
            return vec3(x / l, y / l, z / l);
        }
    };

    // Returns the magnitude of the vector.
    float magnitude() { return sqrt(x * x + y * y + z * z); }

    // Returns the squaredMagnitude of the vector.
    float squaredMagnitude() { return x * x + y * y + z * z; }

    // Inverts the current vectors components.
    void invert()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    // Operators
    // Math

    friend vec3 operator-(const vec3& lhs, const vec3& rhs)
    {
        return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }
    friend vec3 operator+(const vec3& lhs, const vec3& rhs)
    {
        return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    friend vec3 operator*(const vec3& lhs, const vec3& rhs)
    {
        return vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
    }
    friend vec3 operator*(const vec3& lhs, const float amount)
    {
        return vec3(lhs.x * amount, lhs.y * amount, lhs.z * amount);
    }

    friend vec3 operator/(const vec3& lhs, const vec3& rhs)
    {
        if (rhs.x == 0.0 || rhs.y == 0.0 || rhs.z == 0.0)
        {
            std::cout << "Cannot divide by zero, some component of rhs is equal to zero.\nReturning empty vector.\n";
            return vec3();
        }
        return vec3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
    }
    friend vec3 operator/(const vec3& lhs, const float amount)
    {
        if (amount == 0)
        {
            std::cout << "Cannot divide by zero, amount is zero.\nReturning empty vector.\n";
            return vec3();
        }
        return vec3(lhs.x / amount, lhs.y / amount, lhs.z / amount);
    }

    // Output

    friend std::ostream& operator<<(std::ostream& out, const vec3& aVec3)
    {
        out << "X: " << aVec3.x << " Y: " << aVec3.y << " Z: " << aVec3.z;
        return out;
    }

    // Comparison

    friend bool operator==(const vec3& v1, const vec3& v2) { return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z); }

    friend bool operator!=(const vec3& v1, const vec3& v2) { return !(v1 == v2); }
};

// Quaterion
class quaternion
{
public:
    float x;
    float y;
    float z;
    float w;

    quaternion() : x(0), y(0), z(0), w(0) {}
    quaternion(float aX, float aY, float aZ, float aW = 1.0) : x(aX), y(aY), z(aZ), w(aW) {}

    // Static variables
    static quaternion identity(){};

    // Static functions
    void set(float aX, float aY, float aZ, float aW){};

    // Non-static functions
    static void angle(){};
    static void dot(){};
    static void lerp(){};
    static void slerp(){};

    // Operators
    // Math

    friend quaternion operator*(const quaternion& q1, const quaternion& q2)
    {
        return quaternion(q2.x * q1.x - q2.y * q1.y - q2.z * q1.z - q2.w * q1.w,
                          q2.x * q1.y + q2.y * q1.x + q2.z * q1.w - q2.w * q1.z,
                          q2.x * q1.z - q2.y * q1.w + q2.z * q1.x + q2.w * q1.y,
                          q2.x * q1.w + q2.y * q1.z - q2.z * q1.y + q2.w * q1.x);
    }

    // Comparion
    friend bool operator==(const quaternion& q1, const quaternion& q2)
    {
        return (q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w);
    }
    friend bool operator!=(const quaternion& q1, const quaternion& q2)
    {
        return (q1.x != q2.x && q1.y != q2.y && q1.z != q2.z && q1.w != q2.w);
    }
};

// Contains a 3x3 matrix
// class mat3 {};

// Returns the absolute value of f.
float abs(float f) { return std::abs(f); }

// Returns the inverse arc-cosine of f.
float acos(float f) { return ::acos(f); }

// Returns the inverse sin of f.
float asin(float f) { return ::asin(f); }

// Returns the inverse tangent of f.
float atan(float f) { return ::atan(f); }

// Returns the angle in radians whose Tan is y/x.
float atan2(float y, float x) { return ::atan2(y, x); }

// Returns the smallest integer greater to or equal to f.
float ceil(float f) { return ::ceil(f); }

// Clamps a value between a minimum float and a maximum float.
// float clamp(float min, float max) { return 0.0; }

// Returns the cosine of angle f in radians.
float cos(float f) { return ::cos(f); }

// Returns the largest integer smaller to or equal to f.
float floor(float f) { return ::floor(f); }

// Linearly interpolates between point a and b by t, t is clamped by the range [0, 1].
float lerp(float a, float b, float t) { return float((1.0f - t) * a + t * b); }

// Returns the base 10 logarithm of a specified number
float log10(float f) { return ::log10(f); }

// Returns the natural or base-e log
// float ln(float f) { return 0.0; }

// Returns the bigger of the two floats.
float max(float a, float b)
{
    if (a > b)
        return a;
    else if (a < b)
        return b;
    else
        return a;
}

// Returns the smaller of the two floats.
float min(float a, float b)
{
    if (a < b)
        return a;
    else if (a > b)
        return b;
    else
        return a;
}

// Returns the sine of angle f in radians.
float sin(float f) { return ::sin(f); }

// Returns the square root of f.
float sqrt(float f) { return ::sqrt(f); }

// Returns the tangent of angle f in radians.
float tan(float f) { return ::tan(f); }
}

#endif // BML_H