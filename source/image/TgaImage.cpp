#include "TgaImage.hpp"

TgaImage::TgaImage(std::int32_t width, std::int32_t height, std::int32_t bpp, TgaImageColor color) : m_width(width), m_height(height), m_bpp(bpp), data(width * height * bpp, 0)
{
    for (size_t )
}




TGAImage::TGAImage(const int w, const int h, const int bpp, TGAColor c) : w(w), h(h), bpp(bpp), data(w*h*bpp, 0) {
    for (int j=0; j<h; j++)
        for (int i=0; i<w; i++)
            set(i, j, c);
}

TGAColor TGAImage::get(const int x, const int y) const {
    if (!data.size() || x<0 || y<0 || x>=w || y>=h) return {};
    TGAColor ret = {0, 0, 0, 0, bpp};
    const std::uint8_t *p = data.data()+(x+y*w)*bpp;
    for (int i=bpp; i--; ret.bgra[i] = p[i]);
    return ret;
}

void TGAImage::set(int x, int y, const TGAColor &c) {
    if (!data.size() || x<0 || y<0 || x>=w || y>=h) return;
    memcpy(data.data()+(x+y*w)*bpp, c.bgra, bpp);
}

int TGAImage::width() const {
    return w;
}

int TGAImage::height() const {
    return h;
}

