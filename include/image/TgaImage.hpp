#ifndef TGA_IMAGE_HPP
#define TGA_IMAGE_HPP

#include <cstdint>
#include <fstream>
#include <string_view>
#include <vector>
#include "Color.hpp"

enum class TgaImageFormat {
    GRAY_SCALE = 1,
    RGB = 3,
    RGBA = 4,
};

enum class TgaImageDataType : std::uint8_t {
    UNCOMPRESSED_TRUE_COLOR = 2,
    UNCOMPRESSED_GRAY_SCALE = 3,
    RLE_TRUE_COLOR = 10,
    RLE_GRAY_SCALE = 11
};

struct TgaImageDescriptor {
    std::uint8_t value;
    struct {
        std::uint8_t attribute : 4; // Number of attribute bits per pixel, often used to indicate the number of alpha bits
        std::uint8_t horizontalOrigin : 1; // Horizontal origin: 0 left, 1 right
        std::uint8_t verticalOrigin : 1; // Vertical origin: 0 bottom, 1 top
        std::uint8_t interleave : 2; // Interleaving mode: 00 No interleaving, 01 Two-way interleaving, 10 Four-way interleaving, 11 Reserved
    } descriptor;
};

#pragma pack(push, 1)
struct TgaImageHeader {
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
#pragma pack(pop)

struct TgaImageColor {
    std::uint8_t bgra[4] = { 0 };
    std::uint8_t bytespp = 4; // bytes per pixel

    std::uint8_t& operator[](const int32_t i)
    {
        return bgra[i];
    }

    const std::uint8_t& operator[](const int32_t i) const
    {
        return bgra[i];
    }

    explicit operator Color() const;
};

class TgaImage {
public:
    TgaImage() = default;
    TgaImage(std::int32_t width, std::int32_t height, TgaImageFormat format, TgaImageColor color = {});

    std::int32_t GetWidth() const;
    std::int32_t GetHeight() const;
    TgaImageColor GetPixelColor(std::int32_t x, std::int32_t y) const;

    void SetPixelColor(std::int32_t x, std::int32_t y, TgaImageColor& color);
    bool Load(std::string_view path);

private:
    bool LoadHeader(std::ifstream& in, TgaImageHeader& header);
    bool LoadUncompressedData(std::ifstream& in);
    bool LoadRleData(std::ifstream& in);
    void FlipDataHorizontally();
    void FlipDataVertically();

    std::int32_t m_width { 0 };
    std::int32_t m_height { 0 };
    std::uint8_t m_bpp { 0 };
    std::vector<std::uint8_t> m_data {};
};

#endif
