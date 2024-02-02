#include "Precompiled.h"

#include "WindowManager.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

WindowManager* WindowManager::sWindowManager = nullptr;
const TCHAR* WindowManager::sClassName = _T("OAKM_MINECRAFT");
const TCHAR* WindowManager::sTitle = _T("Minecraft Clone");
TCHAR WindowManager::sPlayTitle[256];

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return WindowManager::sWindowManager->msgProc(hwnd, msg, wParam, lParam);
}

WindowManager::WindowManager()
    : mHandle(nullptr)
{
}

WindowManager::~WindowManager()
{
    DestroyWindow(sWindowManager->mHandle);
}

bool WindowManager::CreateInstance(const IntVector2D& screenSize)
{
    ASSERT(sWindowManager == nullptr, "WindowManager::CreateInstance() : instance already created");

    sWindowManager = new WindowManager();

    WNDCLASSEX wcex;
    memset(&wcex, 0, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_CLASSDC;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = GetModuleHandle(nullptr);
    wcex.lpszClassName = sClassName;
    wcex.hIcon = nullptr;
    wcex.hIconSm = nullptr;
    wcex.hbrBackground = nullptr;
    wcex.hCursor = nullptr;
    wcex.lpszMenuName = nullptr;

    if (!::RegisterClassEx(&wcex))
    {
        ASSERT(false, "RegisterClassEx() failed");
        return false;
    }

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = screenSize.mX - 1;
    rect.bottom = screenSize.mY - 1;
    ::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    int windowWidth = rect.right - rect.left + 1;
    int windowHeight = rect.bottom - rect.top + 1;

    sWindowManager->mHandle = ::CreateWindow(sClassName, sTitle, WS_OVERLAPPEDWINDOW,
        (GetSystemMetrics(SM_CXFULLSCREEN) - windowWidth) / 2, (GetSystemMetrics(SM_CYFULLSCREEN) - windowHeight) / 2,
        windowWidth, windowHeight, nullptr, nullptr, wcex.hInstance, nullptr);
    
    if (!sWindowManager->mHandle)
    {
        ASSERT(sWindowManager->mHandle, "CreateWindow() failed");
        return false;
    }

    return true;
}

void WindowManager::DeleteInstance()
{
    ASSERT(sWindowManager, "WindowManager::DeleteInstance() : instance not created");

    if (sWindowManager)
    {
        delete sWindowManager;
        sWindowManager = nullptr;
    }
}

WindowManager& WindowManager::GetInstance()
{
    ASSERT(sWindowManager, "WindowManager::GetInstance() : instance not created");

    return *sWindowManager;
}

void WindowManager::Show() const
{
    ::ShowWindow(mHandle, SW_SHOW);
    ::SetForegroundWindow(mHandle);
    ::SetFocus(mHandle);
}

void WindowManager::CenterWindow() const
{
    RECT rc, rcOwner, rcWindow;
    HWND ownerHandle = GetDesktopWindow();
    GetWindowRect(ownerHandle, &rcOwner);
    GetWindowRect(mHandle, &rcWindow);
    CopyRect(&rc, &rcOwner);

    OffsetRect(&rcWindow, -rcWindow.left, -rcWindow.top);
    OffsetRect(&rc, -rc.left, -rc.top);
    OffsetRect(&rc, -rcWindow.right, -rcWindow.bottom);

    ::SetWindowPos(mHandle, HWND_TOP,
        rcOwner.left + (rc.right / 2),
        rcOwner.top + (rc.bottom / 2),
        0, 0,
        SWP_NOSIZE);
}

LRESULT WindowManager::msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
    {
        return true;
    }

    switch (msg)
    {
        case WM_DISPLAYCHANGE:
        case WM_SIZE:
        {
            break;
        }
        case WM_MOUSEMOVE:
        {
            if (mOnMouseMove)
            {
                mOnMouseMove(*mPlayer, LOWORD(lParam), HIWORD(lParam));
            }

            break;
        }
        case WM_KEYDOWN:
        {
            if (wParam == 27)
            {
                DestroyWindow(hwnd);
            }

            if (mOnKeyboardPress)
            {
                mOnKeyboardPress(*mPlayer, (int)wParam);
            }

            break;
        }
        case WM_KEYUP:
        {
            if (mOnKeyboardRelease)
            {
                mOnKeyboardRelease(*mPlayer, (int)wParam);
            }

            break;
        }
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        case WM_SYSCOMMAND:
        {
            if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER || wParam == SC_KEYMENU)
            {
                return 0;
            }
            break;
        }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool WindowManager::Tick() const
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg); // converts keyboard message to character messages
        DispatchMessage(&msg); // delivers the mssage to winProc

        if (msg.message == WM_QUIT)
        {
            return false;
        }
    }

    return true;
}