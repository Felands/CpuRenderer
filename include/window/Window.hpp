#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <windows.h>
#include <windowsx.h>
#include "Color.hpp"
#include "Geometry.hpp"

constexpr size_t KEY_NUM = 256;

struct Mouse {
    Point2 position;
    bool isLeftButtonDown { false };
    bool isRightButtonDown { false };
};

struct Keyboard {
    bool keys[KEY_NUM] { 0 };
};

class Window {
public:
    static Window& GetInstance()
    {
        static Window window;
        return window;
    }

    bool Init();

    void DeInit();

    unsigned int GetWidth()
    {
        return WINDOW_WIDTH;
    }

    unsigned int GetHeight()
    {
        return WINDOW_HEIGHT;
    }

    unsigned int* GetCanvas()
    {
        return m_canvas;
    }

    bool IsQuit()
    {
        return m_isQuit;
    }

    void Draw(const Point2& point, Color color)
    {
        if (point.x < 0 || point.x >= static_cast<unsigned int>(WINDOW_WIDTH) || point.y < 0 || point.y >= static_cast<unsigned int>(WINDOW_HEIGHT)) {
            return;
        }

        unsigned int x = point.x;
        unsigned int y = point.y;
        m_canvas[y * WINDOW_WIDTH + x] = static_cast<unsigned int>(color);
    }

    void HandleInput();

    void Render();

private:
    Window() {}
    ~Window() {}
    static LRESULT CALLBACK WindowProcess(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT HandleWindowMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    inline static constexpr unsigned int WINDOW_WIDTH = 800;
    inline static constexpr unsigned int WINDOW_HEIGHT = 600;
    inline static constexpr const char* WINDOW_NAME = "CPU Renderer";
    inline static constexpr const char* WINDOW_CLASS_NAME = "CpuRendererWindow";

    bool m_isInit { false };
    bool m_isQuit { false };
    HWND m_hwnd {};
    WNDCLASSA m_windowClass {};
    HDC m_hdc {};
    BITMAPINFO m_bmi {};
    unsigned int m_canvas[WINDOW_WIDTH * WINDOW_HEIGHT] { 0 };

    Mouse m_mouse {};
    Keyboard m_keyboard {};
};

#endif