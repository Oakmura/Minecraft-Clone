#pragma once

#include "Settings.h"
#include "Player.h"
#include "Scene/World.h"
#include "Core/WindowManager.h"
#include "Core/GraphicsEngine.h"
#include "Core/Renderer.h"

class ImGuiUI final
{
public:
    static void Init();
    static void Destroy();
    static ImGuiUI& GetInstance();

public:
    void Update(Renderer& renderer, World& world, Player& player, const float deltaTime);
    void Render() const;
    void OnScreenResize(const IntVector2D& screenSize);

private:
    void startNewFrame(const float deltaTime) const;
    void endNewFrame() const;

private:
    ImGuiUI() = default;
    ~ImGuiUI();
    ImGuiUI(const ImGuiUI& rhs) = delete;
    ImGuiUI& operator=(const ImGuiUI& rhs) = delete;

private:
    static ImGuiUI* sUserInterface;
};