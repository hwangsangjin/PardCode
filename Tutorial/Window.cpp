#include "Window.h"
#include <cassert>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // 윈도우가 생성될 때 실행되는 이벤트
        Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
        window->SetWindowHandle(hwnd);
        window->OnCreate();
        break;
    }
    case WM_DESTROY:
    {
        // 윈도우가 소멸될 때 실행되는 이벤트
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        window->OnDestroy();
        ::PostQuitMessage(0);
        break;
    }
    default:
        return ::DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return NULL;
}

bool Window::Initialize()
{
    // WNDCLASSEX 객체 설정
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = L"Tutorial";
    wc.lpszMenuName = L"";
    wc.style = NULL;
    wc.lpfnWndProc = &WndProc;
    assert(::RegisterClassEx(&wc));

    // 윈도우 생성
    m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"Tutorial", L"Tutorial", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, this);
    assert(m_hwnd);

    // 윈도우 창 보이기
    ::ShowWindow(m_hwnd, SW_SHOW);
    ::UpdateWindow(m_hwnd);

    // 윈도우 실행 플래그를 true로 설정
    m_is_running = true;

    return true;
}

bool Window::Broadcast()
{
    OnUpdate();

    MSG msg;
    while (::PeekMessage(&msg, NULL,0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ::Sleep(1);

    return true;
}

bool Window::IsRunning()
{
    return m_is_running;
}

bool Window::Release()
{
    assert(::DestroyWindow(m_hwnd));
    return true;
}

RECT Window::GetClientWindowRect() const
{
    RECT rc;
    ::GetClientRect(m_hwnd, &rc);
    return rc;
}

void Window::SetWindowHandle(HWND hwnd)
{
    m_hwnd = hwnd;
}

void Window::OnCreate()
{
}

void Window::OnUpdate()
{
}

void Window::OnDestroy()
{
    m_is_running = false;
}
