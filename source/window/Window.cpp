#include "Window.hpp"
#include "Log.hpp"

bool Window::Init()
{
    if (m_isInit) {
        LOG_WARN("Window has been initialized.");
        return true;
    }

    m_windowClass.lpfnWndProc = &Window::WindowProcess;
    m_windowClass.hInstance = GetModuleHandleA(nullptr);
    m_windowClass.lpszClassName = WINDOW_CLASS_NAME;
    RegisterClassA(&m_windowClass);

    m_hwnd = CreateWindowExA(
        0,
        m_windowClass.lpszClassName,
        WINDOW_NAME,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        500, 500, WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr, nullptr, GetModuleHandleA(nullptr), this
    );
    if (m_hwnd == nullptr) {
        LOG_ERROR("Failed to initialize the window, and the error msg is {}.", GetLastError());
        return false;
    }

    m_hdc = GetDC(m_hwnd);

    m_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_bmi.bmiHeader.biWidth = WINDOW_WIDTH;
    m_bmi.bmiHeader.biHeight = -WINDOW_HEIGHT;
    m_bmi.bmiHeader.biPlanes = 1;
    m_bmi.bmiHeader.biBitCount = 32;
    m_bmi.bmiHeader.biCompression = BI_RGB;

    m_isInit = true;
    LOG_INFO("Window initialization successful.");
}

void Window::DeInit()
{
    m_isInit = false;
    DestroyWindow(m_hwnd);
    LOG_INFO("Window deinitialization successful.");
}

void Window::HandleInput()
{
    MSG message {};
    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT) {
            m_isQuit = true;
            return;
        }
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void Window::Render()
{
    StretchDIBits(m_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, m_canvas, &m_bmi, DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK Window::WindowProcess(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window* self = nullptr;

    if (message == WM_NCCREATE) {
        auto* createInfo = reinterpret_cast<CREATESTRUCTA*>(lParam);
        self = static_cast<Window*>(createInfo->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    } else {
        self = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (self) {
        return self->HandleWindowMessage(hwnd, message, wParam, lParam);
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT Window::HandleWindowMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_KEYDOWN: {
            if (wParam < KEY_NUM) {
                m_keyboard.keys[wParam] = true;
            }
            if (wParam == VK_ESCAPE) {
                m_isQuit = true;
            }
            return 0;
        }

        case WM_KEYUP: {
            if (wParam < KEY_NUM) {
                m_keyboard.keys[wParam] = false;
            }
            return 0;
        }

        case WM_MOUSEMOVE: {
            m_mouse.position.x = GET_X_LPARAM(lParam);
            m_mouse.position.y = GET_Y_LPARAM(lParam);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            m_mouse.isLeftButtonDown = true;
            m_mouse.position.x = GET_X_LPARAM(lParam);
            m_mouse.position.y = GET_Y_LPARAM(lParam);
            return 0;
        }

        case WM_LBUTTONUP: {
            m_mouse.isLeftButtonDown = false;
            return 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}