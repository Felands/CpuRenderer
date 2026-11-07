#include "Rasterize.hpp"
#include <algorithm>
#include <cstdint>
#include <cmath>
#include "Window.hpp"

void Rasterize(const Triangle3& ndc, const Triangle2& screen)
{
    static std::vector<double> zbuffer(width * height, -1000.0);

    Matrix<3, 3> ext = {
        {
            { screen.vertexA.x, screen.vertexA.y, 1 },
            { screen.vertexB.x, screen.vertexB.y, 1 },
            { screen.vertexC.x, screen.vertexC.y, 1 },
        }
    };
    
    if (ext.Determinant() / 2.0 < 1) {
        return;
    }

    auto [minx, maxx] = std::minmax({screen.vertexA.x, screen.vertexB.x, screen.vertexC.x});
    auto [miny, maxy] = std::minmax({screen.vertexA.y, screen.vertexB.y, screen.vertexC.y});
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

            double z = barycentricCoordinate * Vector<3>{ ndc[0].z, ndc[1].z, ndc[2].z };
            if (z <= zbuffer[x + y * width]) {
                continue;
            }
            zbuffer[x + y * width] = z;

            
            Window::GetInstance().Draw(x, y, color);
        }
    }
}
