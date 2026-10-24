#ifndef TGA_IMAGE_HPP
#define TGA_IMAGE_HPP

#include <cstdint>
#include <vector>
#include "Geometry.hpp"

struct alignas(1) TgaImageHeader {
    std::uint8_t  idLength = 0;
    std::uint8_t  colorMapType = 0;
    std::uint8_t  dataTypeCode = 0;
    std::uint16_t colorMapOrigin = 0;
    std::uint16_t colorMapLength = 0;
    std::uint8_t  colorMapDepth = 0;
    std::uint16_t xOrigin = 0;
    std::uint16_t yOrigin = 0;
    std::uint16_t width = 0;
    std::uint16_t height = 0;
    std::uint8_t  bitSperPixel = 0;
    std::uint8_t  imageDescriptor = 0;
};

class TgaImageColor {
    std::uint8_t m_bgra[4] = { 0 };
    std::uint8_t m_bytespp = 4; // bytes per pixel

    std::uint8_t& operator[](const int32_t i)
    {
        return bgra[i];
    }

    const std::uint8_t& operator[](const int32_t i) const
    {
        return bgra[i];
    }
};

class TgaImage {
public:
    TgaImage() = default;
    TgaImage(std::int32_t width, std::int32_t height, std::uint8_t bpp, TgaImageColor color = {});

    std::int32_t width() const;
    std::int32_t height() const;
    TgaImageColor GetPixelColor(std::int32_t x, std::int32_t y) const;

    void SetPixelColor(std::int32_t x, std::int32_t y, TgaImageColor& color);

private:
    std::int32_t m_width = 0;
    std::int32_t m_height = 0;
    std::uint8_t m_bpp = 0;
    std::vector<std::uint8_t> m_data = {};
};

#endif
