#include "Precompiled.h"

#include "GraphicsResourceManager.h"
#include "UserInterface.h"
#include "Renderer.h"

UserInterface* UserInterface::sUserInterface = nullptr;

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool UserInterface::CreateInstance(const HWND& windowHandle, ID3D11Device* device, ID3D11DeviceContext* context, const IntVector2D& screenSize)
{
    ASSERT(sUserInterface == nullptr, "UserInterface::CreateInstance() : Instance already created");

    sUserInterface = new UserInterface();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsClassic();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_Init(windowHandle);
    ImGui_ImplDX11_Init(device, context);

    return true;
}

void UserInterface::DeleteInstance()
{
    ASSERT(sUserInterface, "UserInterface::DeleteInstance() : Instance not created");

    if (sUserInterface)
    {
        delete sUserInterface;
        sUserInterface = nullptr;
    }
}

UserInterface& UserInterface::GetInstance()
{
    ASSERT(sUserInterface, "UserInterface::GetInstance() : Instance not created");

    return *sUserInterface;
}

void UserInterface::Update(GraphicsResourceManager& GRM, Renderer& renderer, Player& player)
{
    startNewFrame();
    {
        ImGui::SliderFloat4("background color", renderer.mBackgroundColor, 0.f, 1.f);
        ImGui::SliderFloat4("background color2", renderer.mBackgroundColor2, 0.f, 1.f);

        ImGui::SliderFloat3("camera position", (float*)&player.mPlayerCamera->mPos, -100.f, 100.f);
        ImGui::SliderFloat("camera yaw", &player.mPlayerCamera->mYawInRadian, -100.f, 100.f);
        ImGui::SliderFloat("camera pitch", &player.mPlayerCamera->mPitchInRadian, -100.f, 100.f);
        /*ImGui::SliderFloat3("camera position", (float*)&renderer.mMainCamera.mPos, -5.f, 5.f);
        ImGui::SliderFloat("camera yaw", (float*)&renderer.mMainCamera.mYawInRadian, -3.14f, 3.14f);
        ImGui::SliderFloat("camera pitch", (float*)&renderer.mMainCamera.mPitchInRadian, -3.14f, 3.14f);*/


        bool show_demo_window;
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    endNewFrame();
}

void UserInterface::Render() const
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void UserInterface::OnScreenResize(const IntVector2D& screenSize)
{
}

void UserInterface::startNewFrame() const
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Scene Control");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void UserInterface::endNewFrame() const
{
    ImGui::End();
}
