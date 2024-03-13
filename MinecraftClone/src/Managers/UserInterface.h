#pragma once

#include "Player.h"
#include "World.h"
#include "WindowManager.h"
#include "GraphicsResourceManager.h"
#include "Renderer.h"

class UserInterface final
{
public:
    static bool CreateInstance(const IntVector2D& screenSize);
    static void DeleteInstance();
    static UserInterface& GetInstance();

public:
    void Update(Renderer& renderer, World& world, Player& player);
    void Render() const;
    inline float GetDeltaTime() const { return ImGui::GetIO().DeltaTime; }
    inline float GetFrameRate() const { return ImGui::GetIO().Framerate; }

    void OnScreenResize(const IntVector2D& screenSize);
    inline void BindOnGuiResizeFunc(std::function<void(const int guiWidth)> resizeFunc) { mOnResizeFunc = resizeFunc; }

private:
    void startNewFrame() const;
    void endNewFrame() const;

private:
    UserInterface();
    ~UserInterface();
    UserInterface(const UserInterface& rhs) = delete;
    UserInterface& operator=(const UserInterface& rhs) = delete;

private:
    static UserInterface* sUserInterface;

    std::function<void(const int guiWidth)> mOnResizeFunc;
    int mGuiWidth = 0;
};