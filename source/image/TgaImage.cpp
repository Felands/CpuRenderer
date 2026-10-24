#include "TgaImage.hpp"
#include <cassert>
#include <cerrno>
#include <string>
#include <system_error>
#include "Logger.hpp"

TgaImageColor::operator Color() const
{
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;
    switch (bytespp) {
        case 1:
            red = bgra[0];
            green = bgra[0];
            blue = bgra[0];
            break;
        case 3:
        case 4:
            blue = bgra[0];
            green = bgra[1];
            red = bgra[2];
            break;
        default:
            assert(false && "Unsuppoted TGA pixel size.");
            return Color::BLACK;
    }

    const std::uint32_t packed = 
        (static_cast<std::uint32_t>(red) << 16) |
        (static_cast<std::uint32_t>(green) << 8) |
        (static_cast<std::uint32_t>(blue));

    return static_cast<Color>(packed);
}

TgaImage::TgaImage(std::int32_t width, std::int32_t height, TgaImageFormat format, TgaImageColor color) : m_width(width), m_height(height), m_bpp(static_cast<std::uint8_t>(format))
{
    m_bpp = static_cast<uint8_t>(format);
    m_data.resize(m_width * m_height * m_bpp);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            SetPixelColor(i, j, color);
        }
    }
}

std::int32_t TgaImage::GetWidth() const
{
    return m_width;
}

std::int32_t TgaImage::GetHeight() const
{
    return m_height;
}

TgaImageColor TgaImage::GetPixelColor(std::int32_t x, std::int32_t y) const
{
    if (!m_data.size()) {
        LOG_ERROR("There is no pixel.");
        return {};
    }

    if (x < 0 || x >= m_width) {
        LOG_ERROR("Invalid x.");
        return {};
    }

    if (y < 0 || y >= m_height) {
        LOG_ERROR("Invalid y.");
        return {};
    }

    TgaImageColor color = { 0, 0, 0, 0, m_bpp };
    const std::uint8_t* pData = m_data.data() + (x + y * m_width) * m_bpp;
    for (size_t index = 0; index < m_bpp; index++) {
        color[index] = pData[index];
    }
    return color;
}

void TgaImage::SetPixelColor(std::int32_t x, std::int32_t y, TgaImageColor& color)
{
    if (!m_data.size()) {
        LOG_ERROR("There is no pixel.");
        return;
    }

    if (x < 0 || x >= m_width) {
        LOG_ERROR("Invalid x.");
        return;
    }

    if (y < 0 || y >= m_height) {
        LOG_ERROR("Invalid y.");
        return;
    }

    std::uint8_t* pData = m_data.data() + (x + y * m_width) * m_bpp;
    for (size_t index = 0; index < m_bpp; index++) {
        pData[index] = color[index];
    }
}

bool TgaImage::Load(std::string_view path)
{
    std::ifstream in(std::string(path), std::ios::binary);
    if (!in.is_open()) {
        LOG_ERROR("Failed to open file {}, and errno is {} : '{}'.", path, errno, std::error_code(errno, std::generic_category()).message());
        return false;
    }

    TgaImageHeader header;
    if (!LoadHeader(in, header)) {
        return false;
    }

    m_data = std::vector<std::uint8_t>(m_bpp * m_width * m_height);
    switch (header.dataTypeCode) {
        case static_cast<int>(TgaImageDataType::UNCOMPRESSED_TRUE_COLOR):
        case static_cast<int>(TgaImageDataType::UNCOMPRESSED_GRAY_SCALE):
            if (!LoadUncompressedData(in)) {
                return false;
            }
            break;
        case static_cast<int>(TgaImageDataType::RLE_TRUE_COLOR):
        case static_cast<int>(TgaImageDataType::RLE_GRAY_SCALE):
            if (!LoadRleData(in)) {
                return false;
            }
            break;
        default:
            LOG_ERROR("Unknown data type:{}.", header.dataTypeCode);
            return false;
    }

    TgaImageDescriptor descriptor {};
    descriptor.value = header.imageDescriptor;
    if (descriptor.descriptor.horizontalOrigin) {
        FlipDataHorizontally();
    }
    if (!descriptor.descriptor.verticalOrigin) {
        FlipDataVertically();
    }

    LOG_INFO("Tga image loaded successfully.");
    return true;
}

bool TgaImage::LoadHeader(std::ifstream& in, TgaImageHeader& header)
{
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    LOG_DEBUG(
        "TGA header: idLength={}, colorMapType={}, dataTypeCode={}, "
        "colorMapOrigin={}, colorMapLength={}, colorMapDepth={}, "
        "xOrigin={}, yOrigin={}, width={}, height={}, bitsPerPixel={}, "
        "imageDescriptor={}.",
        static_cast<unsigned int>(header.idLength),
        static_cast<unsigned int>(header.colorMapType),
        static_cast<unsigned int>(header.dataTypeCode),
        header.colorMapOrigin,
        header.colorMapLength,
        static_cast<unsigned int>(header.colorMapDepth),
        header.xOrigin,
        header.yOrigin,
        header.width,
        header.height,
        static_cast<unsigned int>(header.bitSperPixel),
        static_cast<unsigned int>(header.imageDescriptor)
    );
    if (!in.good()) {
        LOG_ERROR("There is an error occured while reading the header.");
        return false;
    }

    m_width = header.width;
    m_height = header.height;
    m_bpp = header.bitSperPixel >> 3;
    if (m_width <= 0 || m_height <= 0 || (m_bpp != static_cast<uint8_t>(TgaImageFormat::GRAY_SCALE) && m_bpp != static_cast<uint8_t>(TgaImageFormat::RGB) && m_bpp != static_cast<uint8_t>(TgaImageFormat::RGBA))) {
        LOG_ERROR("Invalid width or height or bits of per pixel.");
        return false;
    }

    return true;
}

bool TgaImage::LoadUncompressedData(std::ifstream& in)
{
    size_t bytes = m_width * m_height * m_bpp;
    m_data = std::vector<std::uint8_t>(bytes, 0);
    in.read(reinterpret_cast<char*>(m_data.data()), bytes);
    if (!in.good()) {
        LOG_ERROR("There is an occured while reading the data.");
        return false;
    }

    return true;
}

bool TgaImage::LoadRleData(std::ifstream& in)
{
    LOG_DEBUG("Starting to load image data.");

    size_t pixelCount = m_width * m_height;
    size_t currentPixel = 0;
    size_t currentByte = 0;
    TgaImageColor color;
    do {
        std::uint8_t chunkHeader = in.get();
        if (!in.good()) {
            LOG_ERROR("There is an error occured while reading the data.");
            return false;
        }

        if (chunkHeader < 128) {
            chunkHeader++;
            for (size_t index = 0; index < chunkHeader; index++) {
                in.read(reinterpret_cast<char*>(color.bgra), m_bpp);
                if (!in.good()) {
                    LOG_ERROR("There is an error occured while reading the header.");
                    return false;
                }
                for (size_t t = 0; t < m_bpp; t++) {
                    m_data[currentByte++] = color.bgra[t];
                }
                currentPixel++;
                if (currentPixel > pixelCount) {
                    LOG_ERROR("Too many pixels read.");
                    return false;
                }
            }
        } else {
            chunkHeader -= 127;
            in.read(reinterpret_cast<char*>(color.bgra), m_bpp);
            if (!in.good()) {
                LOG_ERROR("There is an error occured while reading the header.");
                return false;
            }
            for (size_t index = 0; index < chunkHeader; index++) {
                for (size_t t = 0; t < m_bpp; t++) {
                    m_data[currentByte++] = color.bgra[t];
                }
                currentPixel++;
                if (currentPixel > pixelCount) {
                    LOG_ERROR("Too many pixels read.");
                    return false;
                }
            }
        }
    } while (currentPixel < pixelCount);
    return true;
}

void TgaImage::FlipDataHorizontally()
{
    for (size_t i = 0; i < m_width / 2; i++) {
        for (size_t j = 0; j < m_height; j++) {
            for (size_t k = 0; k < m_bpp; k++) {
                std::swap(m_data[(i + j * m_width) * m_bpp + k], m_data[(m_width - 1 - i + j * m_width) * m_bpp + k]);
            }
        }
    }
}

void TgaImage::FlipDataVertically()
{
    for (size_t i = 0; i < m_width; i++) {
        for (size_t j = 0; j < m_height / 2; j++) {
            for (size_t k = 0; k < m_bpp; k++) {
                std::swap(m_data[(i + j * m_width) * m_bpp + k], m_data[(i + (m_height - 1 - j) * m_width) * m_bpp + k]);
            }
        }
    }
}
