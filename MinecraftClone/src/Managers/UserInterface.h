#pragma once

class UserInterface final
{
public:
    static bool CreateInstance(const HWND& windowHandle, ID3D11Device* device, ID3D11DeviceContext* context, const IntVector2D& screenSize);
    static void DeleteInstance();
    static UserInterface& GetInstance();

public:
    void Update(GraphicsResourceManager& GRM, Renderer& renderer);
    inline void Render() const { ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); };
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