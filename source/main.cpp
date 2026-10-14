#include <iostream>
#include <windows.h>

#include "Color.hpp"
#include "Geometry.hpp"

constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 600;

uint32_t framebuffer[WIDTH * HEIGHT];

void DrawPoint(uint32_t* buffer, Vector2 position, Color color)
{
    if (position.x < 0 || position.x >= static_cast<int>(WIDTH) ||
        position.y < 0 || position.y >= static_cast<int>(HEIGHT)) {
        return;
    }

    buffer[position.y * WIDTH + position.x] = static_cast<uint32_t>(color);
}

void DrawLine(uint32_t* buffer, Line2 line, Color color)
{
    Vector2 start = line.start;
    Vector2 end = line.end;

    const int dx = std::abs(start.x - end.x);
    const int sx = (start.x < end.x) ? 1 : -1;

    const int dy = -std::abs(start.y - end.y);
    const int sy = (start.y < end.y) ? 1 : -1;

    int error = dx + dy;

    while (true) {
        DrawPoint(buffer, start, color);

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwnd = CreateWindowEx(0, "STATIC", "CPU Renderer", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);

    HDC hdc = GetDC(hwnd);

    Line2 line {
        Vector2(100, 100),
        Vector2(300, 300)
    };
    DrawLine(framebuffer, line, Color::WHITE);

    BITMAPINFO bmi {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = WIDTH;
    bmi.bmiHeader.biHeight = -HEIGHT;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    while(1) {
        StretchDIBits(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, framebuffer, &bmi, DIB_RGB_COLORS, SRCCOPY);
    }
}