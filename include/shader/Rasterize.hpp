#ifndef RASTERIZE_HPP
#define RASTERIZE_HPP

#include <vector>
#include "Geometry.hpp"

std::vector<Point2> Rasterize(const Triangle2& triangle, std::int32_t width, std::int32_t height);

#endif