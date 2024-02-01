#pragma once

class UserInterface final
{
public:
    static void CreateInstance(const HWND& windowHandle, ID3D11Device* device, ID3D11DeviceContext* context, const IntVector2D& screenSize);
    static void DeleteInstance();
    static UserInterface& GetInstance();

public:
    inline void Update(GraphicsResourceManager& GRM);
    inline void Render() const { ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); };
    inline float GetDeltaTime() const { return ImGui::GetIO().DeltaTime; }
    inline float GetFrameRate() const { return ImGui::GetIO().Framerate; }

    void OnScreenResize(const IntVector2D& screenSize);
    inline void BindOnGuiResizeFunc(std::function<void(const int guiWidth)> resizeFunc) { mOnResizeFunc = resizeFunc; }

private:
    inline void startNewFrame() const;
    inline void endNewFrame() const { ImGui::End(); ImGui::Render(); }

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

void UserInterface::startNewFrame() const
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
    ImGui::Begin("Scene Control");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void UserInterface::Update(GraphicsResourceManager& GRM)
{
    startNewFrame();
    {
    }
    endNewFrame();
}