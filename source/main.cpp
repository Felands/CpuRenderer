#include <algorithm>
#include <cstdint>
#include <cmath>
#include <iostream>
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>

#include "Color.hpp"
#include "Geometry.hpp"

constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 600;

uint32_t framebuffer[WIDTH * HEIGHT];
bool keys[256] = {};
int mouseX = 0;
int mouseY = 0;
bool leftMouseDown = false;

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

double EdgeFunction(Vector2 start, Vector2 end, double x, double y)
{
    return (end.x - start.x) * (y - start.y) -
        (end.y - start.y) * (x - start.x);
}

bool IsPointInTriangle(const Triangle2& triangle, double x, double y)
{
    const double area = EdgeFunction(
        triangle.vertexA, triangle.vertexB,
        triangle.vertexC.x, triangle.vertexC.y);
    if (area == 0.0) {
        return false;
    }

    const double edgeAB = EdgeFunction(triangle.vertexA, triangle.vertexB, x, y);
    const double edgeBC = EdgeFunction(triangle.vertexB, triangle.vertexC, x, y);
    const double edgeCA = EdgeFunction(triangle.vertexC, triangle.vertexA, x, y);

    if (area > 0.0) {
        return edgeAB >= 0.0 && edgeBC >= 0.0 && edgeCA >= 0.0;
    }
    return edgeAB <= 0.0 && edgeBC <= 0.0 && edgeCA <= 0.0;
}

void DrawTriangle(uint32_t* buffer, Triangle2 triangle, Color color)
{
    const int minX = std::max(0, std::min({
        triangle.vertexA.x, triangle.vertexB.x, triangle.vertexC.x
    }));
    const int maxX = std::min(static_cast<int>(WIDTH) - 1, std::max({
        triangle.vertexA.x, triangle.vertexB.x, triangle.vertexC.x
    }));
    const int minY = std::max(0, std::min({
        triangle.vertexA.y, triangle.vertexB.y, triangle.vertexC.y
    }));
    const int maxY = std::min(static_cast<int>(HEIGHT) - 1, std::max({
        triangle.vertexA.y, triangle.vertexB.y, triangle.vertexC.y
    }));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            if (IsPointInTriangle(triangle, x + 0.5, y + 0.5)) { // (x + 0.5, y + 0.5) is the pixel center
                DrawPoint(buffer, Vector2{ x, y }, color);
            }
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_KEYDOWN: {
            if (wParam < 256) {
                keys[wParam] = true;
            }
            return 0;
        }

        case WM_KEYUP: {
            if (wParam < 256) {
                keys[wParam] = false;
            }
            return 0;
        }

        case WM_MOUSEMOVE: {
            mouseX = GET_X_LPARAM(lParam);
            mouseY = GET_Y_LPARAM(lParam);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            leftMouseDown = true;
            mouseX = GET_X_LPARAM(lParam);
            mouseY = GET_Y_LPARAM(lParam);
            return 0;
        }

        case WM_LBUTTONUP: {
            leftMouseDown = false;
            return 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSA windowClass {
        .lpfnWndProc = WndProc,
        .hInstance = hInstance,
        .lpszClassName = "CpuRendererWindow"
    };
    RegisterClassA(&windowClass);

    HWND hwnd = CreateWindowExA(
        0,
        windowClass.lpszClassName,
        "CPU Renderer",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, WIDTH, HEIGHT,
        nullptr, nullptr, hInstance, nullptr);

    HDC hdc = GetDC(hwnd);

    DrawTriangle(framebuffer, Triangle2{
        Vector2{ 120, 100 },
        Vector2{ 650, 180 },
        Vector2{ 300, 520 }
    }, Color::WHITE);

    BITMAPINFO bmi {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = WIDTH;
    bmi.bmiHeader.biHeight = -HEIGHT;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    MSG message {};
    while(true) {
        while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                return 0;
            }
            TranslateMessage(&message);
            DispatchMessage(&message);

            if (keys[VK_ESCAPE]) {
                DestroyWindow(hwnd);
            }
        }
        StretchDIBits(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, framebuffer, &bmi, DIB_RGB_COLORS, SRCCOPY);
    }
}