#pragma once

#include "InputManager.h"

class WindowManager final
{
    friend LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    static bool CreateInstance(const IntVector2D& screenSize);
    static void DeleteInstance();
    static WindowManager& GetInstance();

public:
    void Show() const;
    void CenterWindow() const;
    void BindInput(InputManager& inputManager);
    bool Tick() const;

    inline const HWND& GetWindowHandle() const { return mHandle; }
    inline const IntVector2D& GetScreenSize() const { return mScreenSize; }
    inline IntVector2D GetRelativeScreenCenter() { return { mScreenSize.mX >> 1, mScreenSize.mY >> 1 }; }
    inline const IntVector2D& GetAbsoluteScreenCenter() const { return mAbsoluteScreenCenter; }
    inline const RECT& GetWindowBoundary() const { return mWindowRect; }

private:
    WindowManager(const IntVector2D& screenSize);
    ~WindowManager();
    WindowManager(const WindowManager& rhs) = delete;
    WindowManager& operator=(const WindowManager& rhs) = delete;

private:
    LRESULT msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    static WindowManager* sWindowManager;
    static const TCHAR* sClassName;
    static const TCHAR* sTitle;
    static TCHAR sPlayTitle[256];

    HWND mHandle;
    RECT mWindowRect;
    IntVector2D mAbsoluteScreenCenter;
    IntVector2D mScreenSize;

    bool mbMouseMoved;
};