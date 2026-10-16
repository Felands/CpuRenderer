#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cassert>
#include "Log.hpp"

template<unsigned int Dimension>
struct Vector {
    double data[Dimension] { 0 };

    double& operator[](const size_t index)
    {
        assert(index >= 0 && index < Dimension);
        return data[index];
    }
};

template<>
struct Vector<2> {
    double x;
    double y;
};

template<>
struct Vector<3> {
    double x;
    double y;
    double z;
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

template<int Dimension>
double operator*(const Vector<Dimension>& lhs, const Vector<Dimension>& rhs) {
    double ret = 0;
    for (size_t index = 0; index < Dimension; index++) {
        ret += lhs[index] * rhs[index];
    }
    return ret;
}

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
};

template<unsigned int ROW_COUNT, unsigned int COLUM_COUNT>
Vector<ROW_COUNT> operator*(const Matrix<ROW_COUNT, COLUM_COUNT>& lhs, const Vector<COLUM_COUNT>& rhs) {
    Vector<ROW_COUNT> ret {};
    for (size_t index = 0; index < ROW_COUNT; index++) {
        ret[index] = lhs[index] * rhs;
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

#endif