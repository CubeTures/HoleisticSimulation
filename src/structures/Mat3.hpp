#pragma once

#include <cmath>

#include "Vector3.hpp"
#include <raymath.h>

// Row-major 3x3 matrix. m[row][col].
class Mat3 {
  public:
    float m[3][3];

    // Zero matrix by default
    Mat3() : m{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}} {
    }

    Mat3(
        float a00,
        float a01,
        float a02,
        float a10,
        float a11,
        float a12,
        float a20,
        float a21,
        float a22
    )
        : m{{a00, a01, a02}, {a10, a11, a12}, {a20, a21, a22}} {
    }

    // ---- Factories ----

    static Mat3 zero() {
        return diagonal(0, 0, 0);
    }
    static Mat3 identity() {
        return diagonal(1, 1, 1);
    }

    static Mat3 diagonal(float a, float b, float c) {
        return Mat3(a, 0, 0, 0, b, 0, 0, 0, c);
    }

    static Mat3 diagonal(raylib::Vector3 d) {
        return diagonal(d.x, d.y, d.z);
    }

    static Mat3 fromRows(
        raylib::Vector3 r0,
        raylib::Vector3 r1,
        raylib::Vector3 r2
    ) {
        return Mat3(r0.x, r0.y, r0.z, r1.x, r1.y, r1.z, r2.x, r2.y, r2.z);
    }

    static Mat3 fromCols(
        raylib::Vector3 c0,
        raylib::Vector3 c1,
        raylib::Vector3 c2
    ) {
        return Mat3(c0.x, c1.x, c2.x, c0.y, c1.y, c2.y, c0.z, c1.z, c2.z);
    }

    // Skew-symmetric cross-product matrix: skew(a) * b == a x b
    static Mat3 skew(raylib::Vector3 a) {
        return Mat3(0, -a.z, a.y, a.z, 0, -a.x, -a.y, a.x, 0);
    }

    // Outer product: a * b^T
    static Mat3 outer(raylib::Vector3 a, raylib::Vector3 b) {
        return Mat3(
            a.x * b.x,
            a.x * b.y,
            a.x * b.z,
            a.y * b.x,
            a.y * b.y,
            a.y * b.z,
            a.z * b.x,
            a.z * b.y,
            a.z * b.z
        );
    }

    // Rotation matrix from a (unit) quaternion
    static Mat3 fromQuaternion(::Quaternion q) {
        float x = q.x, y = q.y, z = q.z, w = q.w;
        return Mat3(
            1 - 2 * (y * y + z * z),
            2 * (x * y - z * w),
            2 * (x * z + y * w),
            2 * (x * y + z * w),
            1 - 2 * (x * x + z * z),
            2 * (y * z - x * w),
            2 * (x * z - y * w),
            2 * (y * z + x * w),
            1 - 2 * (x * x + y * y)
        );
    }

    // Upper-left 3x3 of a raylib Matrix
    static Mat3 fromMatrix(const ::Matrix &M) {
        return Mat3(M.m0, M.m4, M.m8, M.m1, M.m5, M.m9, M.m2, M.m6, M.m10);
    }

    // ---- Access ----

    float &operator()(int r, int c) {
        return m[r][c];
    }
    float operator()(int r, int c) const {
        return m[r][c];
    }

    float (&operator[](int r))[3] {
        return m[r];
    }
    const float (&operator[](int r) const)[3] {
        return m[r];
    }

    raylib::Vector3 row(int r) const {
        return {m[r][0], m[r][1], m[r][2]};
    }
    raylib::Vector3 col(int c) const {
        return {m[0][c], m[1][c], m[2][c]};
    }

    // ---- Arithmetic ----

    Mat3 operator+(const Mat3 &o) const {
        Mat3 r;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                r.m[i][j] = m[i][j] + o.m[i][j];
        return r;
    }

    Mat3 operator-(const Mat3 &o) const {
        Mat3 r;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                r.m[i][j] = m[i][j] - o.m[i][j];
        return r;
    }

    Mat3 operator-() const {
        return *this * -1.0f;
    }

    Mat3 operator*(float s) const {
        Mat3 r;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                r.m[i][j] = m[i][j] * s;
        return r;
    }

    Mat3 operator/(float s) const {
        return *this * (1.0f / s);
    }

    Mat3 operator*(const Mat3 &o) const {
        Mat3 r;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                r.m[i][j] = m[i][0] * o.m[0][j] + m[i][1] * o.m[1][j] +
                            m[i][2] * o.m[2][j];
        return r;
    }

    raylib::Vector3 operator*(raylib::Vector3 v) const {
        return {
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        };
    }

    Mat3 &operator+=(const Mat3 &o) {
        return *this = *this + o;
    }
    Mat3 &operator-=(const Mat3 &o) {
        return *this = *this - o;
    }
    Mat3 &operator*=(const Mat3 &o) {
        return *this = *this * o;
    }
    Mat3 &operator*=(float s) {
        return *this = *this * s;
    }
    Mat3 &operator/=(float s) {
        return *this = *this / s;
    }

    // ---- Operations ----

    Mat3 transpose() const {
        return Mat3(
            m[0][0],
            m[1][0],
            m[2][0],
            m[0][1],
            m[1][1],
            m[2][1],
            m[0][2],
            m[1][2],
            m[2][2]
        );
    }

    float trace() const {
        return m[0][0] + m[1][1] + m[2][2];
    }

    float determinant() const {
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
               m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
               m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }

    // Returns the zero matrix if singular
    Mat3 inverse() const {
        float det = determinant();
        if (std::fabs(det) < 1e-12f) {
            return Mat3();
        }
        Mat3 adj(
            m[1][1] * m[2][2] - m[1][2] * m[2][1],
            m[0][2] * m[2][1] - m[0][1] * m[2][2],
            m[0][1] * m[1][2] - m[0][2] * m[1][1],

            m[1][2] * m[2][0] - m[1][0] * m[2][2],
            m[0][0] * m[2][2] - m[0][2] * m[2][0],
            m[0][2] * m[1][0] - m[0][0] * m[1][2],

            m[1][0] * m[2][1] - m[1][1] * m[2][0],
            m[0][1] * m[2][0] - m[0][0] * m[2][1],
            m[0][0] * m[1][1] - m[0][1] * m[1][0]
        );
        return adj / det;
    }

    // Re-orthonormalize a rotation matrix (Gram-Schmidt on columns).
    // Useful to fight drift when integrating R directly.
    Mat3 orthonormalize() const {
        raylib::Vector3 c0 = col(0).Normalize();
        raylib::Vector3 c1 = (col(1) - c0 * c0.DotProduct(col(1))).Normalize();
        raylib::Vector3 c2 = c0.CrossProduct(c1);
        return fromCols(c0, c1, c2);
    }

    // Embed into a raylib 4x4 Matrix (column-major), with optional translation
    ::Matrix toMatrix(raylib::Vector3 t = {0, 0, 0}) const {
        ::Matrix M = MatrixIdentity();
        M.m0 = m[0][0];
        M.m4 = m[0][1];
        M.m8 = m[0][2];
        M.m12 = t.x;
        M.m1 = m[1][0];
        M.m5 = m[1][1];
        M.m9 = m[1][2];
        M.m13 = t.y;
        M.m2 = m[2][0];
        M.m6 = m[2][1];
        M.m10 = m[2][2];
        M.m14 = t.z;
        return M;
    }
};

inline Mat3 operator*(float s, const Mat3 &M) {
    return M * s;
}
