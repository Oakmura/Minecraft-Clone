#include "Precompiled.h"

#include "ImGuiUI.h"

ImGuiUI* ImGuiUI::sUserInterface = nullptr;

ImGuiUI::~ImGuiUI()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiUI::Init()
{
    ASSERT(sUserInterface == nullptr, "UserInterface::CreateInstance() : Instance already created");

    sUserInterface = new ImGuiUI();

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

    ImGui_ImplWin32_Init(WindowManager::GetInstance().GetWindowHandle());
    ImGui_ImplDX11_Init(&GraphicsEngine::GetInstance().GetDevice(), &GraphicsEngine::GetInstance().GetDeviceContext());
}

void ImGuiUI::Destroy()
{
    ASSERT(sUserInterface, "UserInterface::DeleteInstance() : Instance not created");

    delete sUserInterface;
    sUserInterface = nullptr;
}

ImGuiUI& ImGuiUI::GetInstance()
{
    ASSERT(sUserInterface, "UserInterface::GetInstance() : Instance not created");

    return *sUserInterface;
}

void ImGuiUI::Update(Renderer& renderer, World& world, Player& player, const float deltaTime)
{
    startNewFrame(deltaTime);
    {
        ImGui::SliderFloat4("background color", renderer.mBackgroundColor, 0.f, 1.f);
        ImGui::Checkbox("Aniso sampler", &renderer.mbAnisoSS);

        ImGui::SliderFloat3("camera position", (float*)&player.mPlayerCamera->mPos, -100.f, 100.f);
        ImGui::SliderFloat3("camera forward", (float*)&player.mPlayerCamera->mForward, -100.f, 100.f);
        ImGui::SliderFloat("camera yaw", &player.mPlayerCamera->mYawInRadian, -100.f, 100.f);
        ImGui::SliderFloat("camera pitch", &player.mPlayerCamera->mPitchInRadian, -100.f, 100.f);

        int a = (int)player.mBlockHandler.mFocusedBlockInfo.BlockType;
        ImGui::SliderInt("focused block", &a, 0, 255);
        ImGui::SliderInt("interaction mode", (int*)&player.mBlockHandler.mInteractionMode, 0, 1);
        ImGui::SliderInt3("block local position", (int*)&player.mBlockHandler.mFocusedBlockInfo.BlockLocalPos, -100, 100);
        ImGui::SliderInt3("block world position", (int*)&player.mBlockHandler.mFocusedBlockWorldPos, -100, 100);
        ImGui::SliderInt3("block normal", (int*)&player.mBlockHandler.mFocusedBlockNormal, -1, 1);

        ImGui::SliderFloat("world fog strength", &world.mGlobalCB.GetCPU().FogStrength, 0.0f, 1.0f);

//         bool show_demo_window;
//         ImGui::ShowDemoWindow(&show_demo_window);
    }
    endNewFrame();
}

void ImGuiUI::Render() const
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

void ImGuiUI::OnScreenResize(const IntVector2D& screenSize)
{
}

void ImGuiUI::startNewFrame(const float deltaTime) const
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Scene Control");
    ImGui::Text("dt: %.3f ms (%.1f FPS)", deltaTime, 1 / deltaTime);
}

void ImGuiUI::endNewFrame() const
{
    ImGui::End();
}
