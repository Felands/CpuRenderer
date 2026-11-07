#ifndef RASTERIZE_HPP
#define RASTERIZE_HPP

#include <vector>
#include "Geometry.hpp"

void Rasterize(const Triangle3& ndc, const Triangle2& screen);

#endif