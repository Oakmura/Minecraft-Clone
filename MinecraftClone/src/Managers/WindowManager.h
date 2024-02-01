#pragma once

class WindowManager final
{
    friend LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    static void CreateInstance(const IntVector2D& screenSize);
    static void DeleteInstance();
    static WindowManager& GetInstance();

public:
    void Show() const;
    void CenterWindow() const;
    inline const HWND& GetWindowHandle() const { return mHandle; }

    bool Tick() const;
    inline void BindOnWindowResizeFunc(std::function<void(const IntVector2D& screenSize)> resizeFunc) { mOnResizeFunc = resizeFunc; }

private:
    WindowManager();
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
    std::function<void(const IntVector2D& screenSize)> mOnResizeFunc;
};