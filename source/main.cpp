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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hwnd = CreateWindowEx(0, "STATIC", "CPU Renderer", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);

    HDC hdc = GetDC(hwnd);

    for (size_t x = 0; x < WIDTH; x++) {
        for (size_t y = 0; y < HEIGHT; y++) {
            DrawPoint(framebuffer, Vector2(x, y), Color::WHITE);
        }
    }

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