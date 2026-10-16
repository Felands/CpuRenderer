#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

template<unsigned int Dimension>
struct Vector {
    double data[Dimension] { 0 };
};

template<>
struct Vector<2> {
    double x;
    double y;
};

template<>
struct Vector<4> {
    double x;
    double y;
    double z;
    double w;
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

template<unsigned int ROW_COUNT, unsigned int COLUM_COUNT>
struct Matrix {
    Vector<COLUM_COUNT> data[ROW_COUNT]{{}};
};

using Matrix4_4 = Matrix<4, 4>;

constexpr Matrix4_4 IDENTITY_MATRIX_4 = {
    {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    }
};

#endif