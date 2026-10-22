#include <algorithm>
#include <cstdint>
#include <cmath>

#include "Color.hpp"
#include "Geometry.hpp"
#include "VertexShader.hpp"
#include "Window.hpp"

void DrawPoint(const Point2& point, Color color)
{
    Window::GetInstance().Draw(point, color);
}

void DrawLine(const Line2& line, Color color)
{
    Point2 start = line.start;
    Point2 end = line.end;

    const int dx = std::abs(start.x - end.x);
    const int sx = (start.x < end.x) ? 1 : -1;

    const int dy = -std::abs(start.y - end.y);
    const int sy = (start.y < end.y) ? 1 : -1;

    int error = dx + dy;

    while (true) {
        DrawPoint(start, color);

        if (start.x == end.x && start.y == end.y) {
            break;
        }

        const int twiceError = 2 * error;
        if (twiceError >= dy) {
            error += dy;
            start.x += sx;
        }

        if (twiceError <= dx) {
            error += dx;
            start.y += sy;
        }
    }
}

inline double EdgeFunction(const Point2& start, const Point2& end, const Point2& point)
{
    return (end.x - start.x) * (point.y - start.y) - (end.y - start.y) * (point.x - start.x);
}

bool IsPointInTriangle(const Triangle2& triangle, const Point2& point)
{
    const double area = EdgeFunction(triangle.vertexA, triangle.vertexB, triangle.vertexC);
    if (area == 0.0) {
        return false;
    }

    const double edgeAB = EdgeFunction(triangle.vertexA, triangle.vertexB, point);
    const double edgeBC = EdgeFunction(triangle.vertexB, triangle.vertexC, point);
    const double edgeCA = EdgeFunction(triangle.vertexC, triangle.vertexA, point);

    if (area > 0.0) {
        return edgeAB >= 0.0 && edgeBC >= 0.0 && edgeCA >= 0.0;
    }
    return edgeAB <= 0.0 && edgeBC <= 0.0 && edgeCA <= 0.0;
}

void DrawTriangle(Triangle2 triangle, Color color)
{
    const int minX = std::min({triangle.vertexA.x, triangle.vertexB.x, triangle.vertexC.x});
    const int maxX = std::max({triangle.vertexA.x, triangle.vertexB.x, triangle.vertexC.x});
    const int minY = std::min({triangle.vertexA.y, triangle.vertexB.y, triangle.vertexC.y});
    const int maxY = std::max({triangle.vertexA.y, triangle.vertexB.y, triangle.vertexC.y});

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            if (IsPointInTriangle(triangle, Point2{ x + 0.5, y + 0.5})) { // (x + 0.5, y + 0.5) is the pixel center
                DrawPoint(Point2{ static_cast<double>(x), static_cast<double>(y) }, color);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if (!Window::GetInstance().Init()) {
        return -1;
    }

    if (!VertexShader::GetInstance().Init()) {
        return -1;
    }

    // 
    Triangle3 triangle {
        Point3 { 50, 250, -50 },
        Point3 { 250, 250, -100 },
        Point3 { 50, 50, -150 }
    };
    VertexShader::GetInstance().Transform(triangle);
    DrawTriangle(Triangle2 {
        Point2 { triangle.vertexA.x, triangle.vertexA.y },
        Point2 { triangle.vertexB.x, triangle.vertexB.y },
        Point2 { triangle.vertexC.x, triangle.vertexC.y }
    }, Color::WHITE);

    while(true) {
        Window::GetInstance().HandleInput();
        if (Window::GetInstance().IsQuit()) {
            break;
        }

        Window::GetInstance().Render();
    }

    Window::GetInstance().DeInit();
}