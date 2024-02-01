#include "Precompiled.h"

#include "GraphicsResourceManager.h"
#include "UserInterface.h"

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.DisplaySize = ImVec2(float(screenSize.mX), float(screenSize.mY));
    ImGui::StyleColorsClassic();

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

void UserInterface::Update(GraphicsResourceManager& GRM)
{
    bool hi = false;
    startNewFrame();
    {
        ImGui::Checkbox("Use Shading", &hi);
    }
    endNewFrame();
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
    ImGui::Render();
}
