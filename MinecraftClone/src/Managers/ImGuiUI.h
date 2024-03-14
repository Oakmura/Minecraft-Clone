#pragma once

#include "Player.h"
#include "World.h"
#include "WindowManager.h"
#include "GraphicsResourceManager.h"
#include "Renderer.h"

class ImGuiUI final
{
public:
    static bool CreateInstance(const IntVector2D& screenSize);
    static void DeleteInstance();
    static ImGuiUI& GetInstance();

public:
    void Update(Renderer& renderer, World& world, Player& player);
    void Render() const;
    inline float GetDeltaTime() const { return ImGui::GetIO().DeltaTime; }
    inline float GetGlobalTime() const { return static_cast<float>(ImGui::GetTime()); }
    inline float GetFrameRate() const { return ImGui::GetIO().Framerate; }

    void OnScreenResize(const IntVector2D& screenSize);
    inline void BindOnGuiResizeFunc(std::function<void(const int guiWidth)> resizeFunc) { mOnResizeFunc = resizeFunc; }

private:
    void startNewFrame() const;
    void endNewFrame() const;

private:
    ImGuiUI();
    ~ImGuiUI();
    ImGuiUI(const ImGuiUI& rhs) = delete;
    ImGuiUI& operator=(const ImGuiUI& rhs) = delete;

private:
    static ImGuiUI* sUserInterface;

    std::function<void(const int guiWidth)> mOnResizeFunc;
    int mGuiWidth = 0;
};