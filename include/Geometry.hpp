#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

struct Vector2 {
    int x;
    int y;
};

struct Line2 {
    Vector2 start;
    Vector2 end;
};

struct Triangle2 {
    Vector2 vertexA;
    Vector2 vertexB;
    Vector2 vertexC;
};

#endif