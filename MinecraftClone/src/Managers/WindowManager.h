#pragma once

#include "Player.h"

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
    inline const HWND& GetWindowHandle() const { return mHandle; }

    bool Tick() const;
    inline void BindMouseMoveFunc(std::function<void(Player& player, const int mouseX, const int mouseY)> mouseMoveFunc) { mOnMouseMove = mouseMoveFunc; }
    inline void BindKeyboardPressFunc(std::function<void(Player& player, const int keyCode)> keyPressFunc) { mOnKeyboardPress = keyPressFunc; }
    inline void BindKeyboardReleaseFunc(std::function<void(Player& player, const int keyCode)> keyReleaseFunc) { mOnKeyboardRelease = keyReleaseFunc; }
    inline void BindPlayer(Player* player) { mPlayer = player; }

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
    std::function<void(Player& player, const int mouseX, const int mouseY)> mOnMouseMove;
    std::function<void(Player& player, const int keyCode)> mOnKeyboardPress;
    std::function<void(Player& player, const int keyCode)> mOnKeyboardRelease;

    Player* mPlayer;
};