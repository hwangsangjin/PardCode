#include "Window.h"
#include <cassert>

Window* g_window = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CREATE:
        // �����찡 ������ �� ����Ǵ� �̺�Ʈ
        g_window->OnCreate();
        break;

    case WM_DESTROY:
        //�����찡 �Ҹ�� �� ����Ǵ� �̺�Ʈ
        g_window->OnDestroy();
        ::PostQuitMessage(0);
        break;

    default:
        return ::DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return NULL;
}

bool Window::Initialize()
{
    // WNDCLASSEX ��ü ����
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

    // ������ ���ν������� ����� �� �ֵ��� ���� ������ ������ �ּ� ����
    assert(!g_window);
    g_window = this;

    // ������ ����
    m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"Tutorial", L"Tutorial", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, this);
    assert(m_hwnd);

    // ������ â ���̱�
    ::ShowWindow(m_hwnd, SW_SHOW);
    ::UpdateWindow(m_hwnd);

    // ������ ���� �÷��׸� true�� ����
    m_is_running = true;

    return true;
}

bool Window::Broadcast()
{
    MSG msg;
    while (::PeekMessage(&msg, NULL,0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    g_window->OnUpdate();

    ::Sleep(0);

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
