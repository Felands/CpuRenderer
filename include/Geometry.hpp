#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cassert>
#include <cmath>
#include "Logger.hpp"

template<unsigned int Dimension>
struct Vector {
    double data[Dimension] { 0 };

    double& operator[](const size_t index)
    {
        assert(index >= 0 && index < Dimension);
        return data[index];
    }

    const double& operator[](const size_t index) const
    {
        assert(index >= 0 && index < Dimension);
        return data[index];
    }
};

template<>
struct Vector<2> {
    double x;
    double y;

    double& operator[](const size_t index)
    {
        assert(index >= 0 && index < 2);
        if (index == 0) return x;
        if (index == 1) return y;
    }

    double operator[](const size_t index) const
    {
        assert(index >= 0 && index < 2);
        if (index == 0) return x;
        if (index == 1) return y;
    }
};

template<>
struct Vector<3> {
    double x;
    double y;
    double z;

    double& operator[](const size_t index)
    {
        assert(index >= 0 && index < 3);
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
    }

    double operator[](const size_t index) const
    {
        assert(index >= 0 && index < 3);
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
    }
};

template<>
struct Vector<4> {
    double x;
    double y;
    double z;
    double w;

    double& operator[](const size_t index)
    {
        assert(index >= 0 && index < 4);
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        if (index == 3) return w;
    }

    double operator[](const size_t index) const
    {
        assert(index >= 0 && index < 4);
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        if (index == 3) return w;
    }
};

template<int Dimension>
Vector<Dimension> operator-(const Vector<Dimension>& lhs, const Vector<Dimension>& rhs) {
    Vector<Dimension> ret = lhs;
    for (size_t index = 0; index < Dimension; index++) {
        ret[index] -= rhs[index];
    }
    return ret;
}

template<int Dimension>
double operator*(const Vector<Dimension>& lhs, const Vector<Dimension>& rhs) {
    double ret = 0;
    for (size_t index = 0; index < Dimension; index++) {
        ret += lhs[index] * rhs[index];
    }
    return ret;
}

template<int Dimension>
Vector<Dimension> operator/(const Vector<Dimension>& lhs, const double& rhs)
{
    Vector<Dimension> ret = lhs;
    for (size_t index = 0; index < Dimension; index++) {
        ret[index] /= rhs;
    }
    return ret;
}

template<int Dimension>
double Length(const Vector<Dimension>& vec)
{
    return std::sqrt(vec * vec);
}

template<int Dimension>
Vector<Dimension> Normalize(const Vector<Dimension>& vec)
{
    return vec / Length(vec);
}

inline Vector<3> Cross(const Vector<3>& vec1, const Vector<3>& vec2)
{
    return {
        vec1.y * vec2.z - vec1.z * vec2.y,
        vec1.z * vec2.x - vec1.x * vec2.z,
        vec1.x * vec2.y - vec1.y * vec2.x
    };
}

struct OrthonormalBasis3 {
    Vector<3> xAxis;
    Vector<3> yAxis;
    Vector<3> zAxis;
};

inline OrthonormalBasis3 BuildOrthonormalBasis(const Vector<3>& lhs, const Vector<3>& rhs)
{
    Vector<3> zAxis = Normalize(rhs);
    Vector<3> xAxis = Normalize(Cross(lhs, rhs));
    Vector<3> yAxis = Normalize(Cross(xAxis, zAxis));
    return OrthonormalBasis3{ xAxis, yAxis, zAxis };
}

using Point2 = Vector<2>;

struct Line2 {
    Point2 start;
    Point2 end;
};

struct Triangle2 {
    Point2 vertexA;
    Point2 vertexB;
    Point2 vertexC;
};

using Point3 = Vector<3>;

struct Line3 {
    Point3 start;
    Point3 end;
};

struct Triangle3 {
    Point3 vertexA;
    Point3 vertexB;
    Point3 vertexC;
};

using Point4 = Vector<4>;

template<unsigned int ROW_COUNT, unsigned int COLUM_COUNT>
struct Matrix {
    Vector<COLUM_COUNT> data[ROW_COUNT]{{}};

    Matrix& operator=(const Matrix& matrix)
    {
        for (size_t row = 0; row < ROW_COUNT; row++) {
            for (size_t col = 0; col < COLUM_COUNT; col++) {
                data[row][col] = matrix[row][col];
            }
        }
        return *this;
    }

    Vector<COLUM_COUNT>& operator[](const size_t index)
    {
        assert(index >= 0 && index < ROW_COUNT);
        return data[index];
    }

    const Vector<COLUM_COUNT>& operator[](const size_t index) const
    {
        assert(index >= 0 && index < ROW_COUNT);
        return data[index];
    }

    double Determinant() const
    {
        static_assert(ROW_COUNT == COLUM_COUNT, "Determinant requires a square matrix");
        static_assert(ROW_COUNT > 0, "Matrix dimension must be positive");

        if constexpr (ROW_COUNT == 1) {
            return data[0][0];
        } else {
            double result = 0.0;
            for (size_t col = 0; col < COLUM_COUNT; ++col) {
                Matrix<ROW_COUNT - 1, COLUM_COUNT - 1> minor{};

                for (size_t row = 1; row < ROW_COUNT; ++row) {
                    size_t minorCol = 0;

                    for (size_t sourceCol = 0; sourceCol < COLUM_COUNT; ++sourceCol) {
                        if (sourceCol == col) {
                            continue;
                        }

                        minor[row - 1][minorCol++] = data[row][sourceCol];
                    }
                }

                const double sign = (col % 2 == 0) ? 1.0 : -1.0;
                result += sign * data[0][col] * minor.Determinant();
            }
            return result;
        }
    }

    double Cofactor(const size_t row, const size_t col) const
    {
        Matrix<ROW_COUNT - 1, COLUM_COUNT - 1> cofactorMatrix;
        size_t coI = 0;
        size_t coJ = 0;
        for (size_t i = 0; i < ROW_COUNT; i++) {
            if (i == row) {
                continue;
            }

            for (size_t j = 0; j < COLUM_COUNT; j++) {
                if (j == col) {
                    continue;
                }

                cofactorMatrix[coI++][coJ++] = data[i][j]; 
            }
        }
        return cofactorMatrix.Determinant() * ((row + col) % 2 ? -1 : 1);
    }

    Matrix<ROW_COUNT, COLUM_COUNT> InvertTranspose() const
    {
        Matrix<ROW_COUNT, COLUM_COUNT> adjugateTranspose;
        for (size_t i = 0; i < ROW_COUNT; i++) {
            for (size_t j = 0; j < COLUM_COUNT; j++) {
                adjugateTranspose[i][j] = Cofactor(i, j);
            }
        }
        return adjugateTranspose / Determinant();
    }
};

template<unsigned int ROW_COUNT, unsigned int COLUM_COUNT>
Vector<ROW_COUNT> operator*(const Matrix<ROW_COUNT, COLUM_COUNT>& lhs, const Vector<COLUM_COUNT>& rhs) {
    Vector<ROW_COUNT> ret {};
    for (size_t index = 0; index < ROW_COUNT; index++) {
        ret[index] = lhs[index] * rhs;
    }
    return ret;
}

template<unsigned int N1, unsigned int N2, unsigned int N3>
Matrix<N1, N3> operator*(const Matrix<N1, N2>& lhs, const Matrix<N2,N3>& rhs)
{
    Matrix<N1,N3> ret {};
    for (size_t i = 0; i < N1; i++) {
        for (size_t j = 0; j < N3; j++) {
            for (size_t k = 0; k < N2; k++) {
                ret[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }
    return ret;
}

template<unsigned int ROW_COUNT, unsigned int COLUM_COUNT>
Matrix<ROW_COUNT, COLUM_COUNT> operator/(const Matrix<ROW_COUNT, COLUM_COUNT>& lhs, const double value) {
    Matrix<ROW_COUNT, COLUM_COUNT> ret {};
    for (size_t index = 0; index < ROW_COUNT; index++) {
        ret[index] = lhs[index] / value;
    }
    return ret;
}

using Matrix4_4 = Matrix<4, 4>;

const Matrix4_4 IDENTITY_MATRIX_4 = {
    {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    }
};

const Matrix4_4 SCALE_HALF_MATRIX_4 = {
    {
        { 0.5, 0, 0, 0 },
        { 0, 0.5, 0, 0 },
        { 0, 0, 0.5, 0 },
        { 0, 0, 0, 1 }
    }
};

const Matrix4_4 ROTATION_LEFT_90_MATRIX_4 = {
    {
        { 0, 0, 1, 0 },
        { 0, 1, 0, 0 },
        { -1, 0, 0, 0 },
        { 0, 0, 0, 1 }
    }
};

const Matrix4_4 TRANSLATION_RIGHT_500_MATRIX_4 = {
    {
        { 1, 0, 0, 500 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    }
};

#endif