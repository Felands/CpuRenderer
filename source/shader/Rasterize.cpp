#include "Rasterize.hpp"
#include <algorithm>
#include <cstdint>
#include <cmath>

std::vector<Point2> Rasterize(const Triangle2& triangle, std::int32_t width, std::int32_t height)
{
    std::vector<Point2> ret;

    Matrix<3, 3> ext = {
        {
            { triangle.vertexA.x, triangle.vertexA.y, 1 },
            { triangle.vertexB.x, triangle.vertexB.y, 1 },
            { triangle.vertexC.x, triangle.vertexC.y, 1 },
        }
    };
    
    if (ext.Determinant() / 2.0 < 1) {
        return ret;
    }

    auto [minx, maxx] = std::minmax({triangle.vertexA.x, triangle.vertexB.x, triangle.vertexC.x});
    auto [miny, maxy] = std::minmax({triangle.vertexA.y, triangle.vertexB.y, triangle.vertexC.y});
    minx = std::max<int>(minx, 0);
    maxx = std::min<int>(maxx, width - 1);
    miny = std::max<int>(miny, 0);
    maxy = std::min<int>(maxy, height - 1);
    for (size_t x = minx; x <= maxx; x++) {
        for (size_t y = miny; y <= maxy; y++) {
            Vector<3> barycentricCoordinate = ext.InvertTranspose() * Vector<3>{static_cast<double>(x), static_cast<double>(y), 1.0};
            if (barycentricCoordinate.x < 0 || barycentricCoordinate.y < 0 || barycentricCoordinate.z < 0) {
                continue;
            }
            ret.emplace_back(Point2{static_cast<double>(x), static_cast<double>(y)});
        }
    }
    return ret;
}